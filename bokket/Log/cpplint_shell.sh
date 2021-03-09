#! /bin/bash
echo "^@^cpplint code style check through shell====^@^"
index=
config=""
pwd_path=`pwd`
cpplint_path="$pwd_path/cpplint.py"
echo cpplint_path=$cpplint_path
 
src_path="$pwd_path/Log"
echo src_path=$src_path
# add file to an array,and check file in array last
# for file in `find $src_path -name "*.h" -type f`
for file in `find $src_path -maxdepth  -type f | grep -E "\.h$|\.cc$|\.cu$|\.cpp$"`
do
    echo file=$file
    echo -e "\033[36m ===> [FILE] \033[0m \033[47;31m $file \033[0m"
    check_files[$index]=$file
    index=$(($index+))
done
# accroding to config options make a checking command
# first check if python2 exists
check_cmd=""
is_python2_exists=`ls /usr/bin/ | grep -E '^python2$' | wc -l`
if [ $is_python2_exists -ge  ]
then
    # read cpplint.ini to decide which option to add
    for file in ${check_files[*]}
    do
        check_cmd="python2 $cpplint_path --linelength=80"
        echo -e "\033[33m =========== check file $file =============\033[0m"
        check_cmd="$check_cmd"$file
        eval $check_cmd
        echo -e "\033[45m ==========================================\033[0m"
    done
fi
