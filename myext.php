<?php

$object = new MyClass;
$object->aPublicProperty = 42;
$object::$aStaticPublicProperty++;

$object2 = new MyClass;
$object2::$aStaticPublicProperty++;

$object3 = new MyClass;
$object3::$aStaticPublicProperty++;

var_dump( $object, $object2, $object3, MyClass::$aStaticPublicProperty );
