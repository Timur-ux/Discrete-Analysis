if [ "$1" == "build" ]; then
  echo "$1"
  make
fi

./build/CP/CP_main_proj ./file1.txt ./file2.txt

