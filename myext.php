<?php

inspect( null );
inspect( true );
inspect( 42 );
inspect( 42.42 );
inspect( "Forty two" );
inspect( array( 1, "2", "three" ) );
inspect( new stdClass );

inspect( fopen( __FILE__, "r" ) );
