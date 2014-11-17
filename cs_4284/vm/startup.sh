#VBoxHeadless --startvm "slave"
#sleep 5i
VBoxManage guestproperty get slave "/VirtualBox/GuestInfo/Net/0/V4/IP" | grep -o -E '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}'
vmIP=$(VBoxManage guestproperty get slave "/VirtualBox/GuestInfo/Net/0/V4/IP" | grep -o -E '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}')
ssh -i ~/Documents/id_rsa frosty@$vmIP << EOF
	echo "Hello world";
	exit;
EOF

