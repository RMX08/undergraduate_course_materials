<?php
class mylogin {
    var $user;
    var $pass;

    function __construct($user, $pass) {
        $this->user = $user;
        $this->pass = $pass;
    }
}

$a = new mylogin("admin", "myzS@11wawq");
echo serialize($a);
?>