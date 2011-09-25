<?php

function fibo($n) {
    switch ($n)
    {
        case 0:
        case 1:
            return $n;
    }

    return fibo($n-2) + fibo($n-1);
}

$n = 35;

echo "PHP userland implementation\nfibo($n) = ";
$t0 = microtime( true );
echo fibo($n), "\n";
echo "Computed in ", microtime( true ) - $t0, " s\n";

echo "PHP userland implementation\nfibo($n) = ";
$t0 = microtime( true );
echo fibonacci($n), "\n";
echo "Computed in ", microtime( true ) - $t0, " s\n";
