#!/bin/bash


if [ $# -ne 3 ];then
	echo "input argument"
	echo "ecryptfs <uuid> <src_dir> <dest_dir> "
fi

sudo ecryptfsd
sudo mount -t ecryptfs $2 $3 << EOF
1
$1
1
1
n
n
EOF
cd door_decrypted
sudo python open.py
sudo python button.py
cd ..
sudo umount door_decrypted