<?php
//��ȡ�������Ϣ
$wifipwd= $_POST["signin-password"];
$wifissid= $_POST["signin-ssid"];
echo 'WIFI Set ssid is '.$wifissid .'password is:  '.$wifipwd ."\n";
echo $_SERVER['SERVER_NAME']."\n";
system("connect2ap $wifissid $wifipwd");
?>