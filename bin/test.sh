root_dirname="compilador"
cafezinho_ext=".caf"
expected_ext=".expected"
comparison_file="comparison.log"

# do not change this
root_path=`pwd`"/"
bin_path=`pwd`
####################

if [[ "${PWD##*/}" == *"$root_dirname" ]]
then
	bin_path+="/bin"
else 
	if [[ "${PWD##*/}" != *"bin" ]]
	then
		echo "Run this command from project's root directory '$root_dirname' it's child directory 'bin'"
		exit 113
	fi
	root_path=`realpath $root_path"/../"`"/"
fi

test_path=$root_path"test/"
success_test=$test_path"success/"
failure_test=$test_path"failure/"

compiler_file=$bin_path"/a.out"


red='\033[0;31m'
green='\033[0;32m'
yellow='\033[4;49;33m'
no_color='\033[0m'

has_errors=false

get_relative_path () {
	full_path=`pwd`
	test_root_index=`echo $full_path | grep -b -o $test_path | awk 'BEGIN{FS=":"}{print $1}'`
	test_path_len=${#test_path}
	((test_root_index+=test_path_len))
	relative_path="`echo ${full_path:$test_root_index}`"
}
crawl () {
	directory=$1
        mode=$2 # 0 for failure tests; 1 for success tests
	cd $directory
	rm -f *.actual
	files=`ls`

	error_index=0
	pending_index=0

	declare -a ftests_failed
	declare -a ftests_pending

	for f in $files
	do
		if [ -d $f ]
		then
			crawl $f
		else
      if [[ $f == *$cafezinho_ext ]] # is a cafezinho source file
      then
        index=`echo $f | grep -b -o ".caf" | awk 'BEGIN{FS=":"}{print $1}'`
        filename=`echo ${f:0:$index}`
        $compiler_file $filename$cafezinho_ext 2>&1 | tail -1 > $comparison_file
        if [ ! -f $filename$expected_ext ] # .expected not found
        then
          if [ $mode == 1 ] # does not need .expected
          then
            difference=`diff $comparison_file /dev/null`
            if [ -n "$difference" ] # test has failed
            then
            	echo -ne "${red}F${no_color}"
            	has_errors=true
              echo $difference > $filename".actual"
              ftests_failed[error_index]=$filename$cafezinho_ext
              ((error_index++))
            else
          		echo -n "."
            fi
          else
            ftests_pending[pending_index]=$filename$cafezinho_ext
            ((pending_index++))
          fi
        else
          difference=`diff $comparison_file $filename$expected_ext`
          if [ -n "$difference" ] # test has failed
          then
          	has_errors=true
            echo -ne "${red}F${no_color}"
            echo $difference > $filename".actual"
            ftests_failed[error_index]=$filename$cafezinho_ext
            ((error_index++))
          else
          	echo -n "."
          fi
        fi

        rm -f $comparison_file
      fi
		fi
	done
	number_of_failures=`echo ${#ftests_failed[@]}`
	if [[ "$number_of_failures" != "0" ]]
	then
		echo -e "\n"
		
		get_relative_path
		echo -e "${yellow}$relative_path${no_color}"
		echo -e "${red}$number_of_failures tests failed:${no_color}"
	fi
	for i in "${ftests_failed[@]}"
	do
		echo -e "\t${red}$i${no_color}"
	done

	number_of_pendings=`echo ${#ftests_pending[@]}`
	if [[ "$number_of_pendings" != "0" ]]
	then
		echo -e "\n"
		
		get_relative_path
		echo -e "${yellow}$relative_path${no_color}"
		echo -e "$number_of_pendings tests pending:"
	fi
	for i in "${ftests_pending[@]}"
	do
		echo -e "\t$i"
	done

	cd ..
}

# test for failure tests.
cd $failure_test
failure_tfiles=`ls $failure_test`

for f in $failure_tfiles
do
	if [ -d $f ]
	then # is a directory
		crawl $f 0
	else # is not a directory
		echo "not a directory:"$f
                crawl $f
	fi
done
echo -e "\n1st Test Phase Done."
###########################
cd $success_test

# test for success tests.
success_tfiles=`ls $success_test`

for f in $success_tfiles
do
	if [ -d $f ]
	then # is a directory
		crawl $f 1
	else # is not a directory
		echo "not a directory:"$f
	fi
done
echo -e "\n2nd Test Phase Done."
###########################

if [ "$has_errors" = false ]
then
	echo -e "${green}All tests have passed!!${no_color}"
fi