<?
include_once ("Commands.php");

class Sockets
{
    var $MESSAGE_END = "";
    var $Commmands;
    var $doNotify;
    var $logfileName = "configuratorLog";
    function Sockets()
    {
        $this->Commands = new Commands();
        $this->doNotify = false;
    }
    
    function socketConnect( &$sock, $ip, $port, $printDebug )
    {
        error_reporting( E_ALL ^ E_WARNING );
        $ret = false;
        $sock = socket_create( AF_INET, SOCK_STREAM, getprotobyname( "tcp" ) );
        $ret = socket_connect( $sock, $ip, $port );
        error_reporting( E_ALL );
        return $ret;
    }

    function socketWrite( $sock, $string, $printDebug ) 
    {
        if( $printDebug  && $this->doNotify )
        {
            $log = fopen( $this->logfileName, "a" );
            fwrite( $log, "Notify: in socketWrite\n" );
            fclose( $log );
        }
        
        $bytes = 0;
        $commandString = $string . $this->Commands->endMessageBlock;
        socket_send( $sock, $commandString, strlen( $commandString ), 0 );
        if( $printDebug && $this->doNotify )
        {
            $log = fopen( $this->logfileName, "a" );
            fwrite( $log, "Notify: leaving socketWrite\n" );
            fclose( $log );
        }
        return true;
    }

    function socketRecv( $sock, &$reply, $printDebug )
    {
        if( $printDebug && $this->doNotify )
        {
            $log = fopen( $this->logfileName, "a" );
            fwrite( $log, "Notify, in SocketRecv\n" );
            fclose( $log );
        }
        $reply = "";
        $bufSize = 20;
        $position = false;
        while( substr_count( $reply, $this->Commands->endMessageBlock ) < 1 ) 
        {
            $temp = "";
            if( ($error = socket_recv( $sock, $temp, $bufSize, 0 )) < 0 )
            {
                if( $printDebug )
                {
                    echo "Error: in socket read, error ".socket_strerror($error)." <br />";
                }
                return false;
            }
            $reply .= $temp;
            if( $printDebug && $this->doNotify )
            {
                $log = fopen( $this->logfileName, "a" );
                fwrite( $log, "Notify: in recv, reply so far: $reply\n" );
                fclose( $log );
            }
        }
        $reply = substr( $reply, 0, strpos( $reply, $this->Commands->endMessageBlock ) );
        if( $printDebug && $this->doNotify )
        {
            $log = fopen( $this->logfileName, "a" );
            fwrite( $log, "Notify: leaving socket recv\n" );
            fclose( $log );
       }
        return true;
    }

    function setNotify( $notify )
    {
        if( $this->doNotify )
        {
            $log = fopen( $this->logfileName, "a" );
            fwrite( $log, "Notify: in setNotify\n" );
            fclose( $log );
        }

        $this->doNotify = $notify;
    }
}
?>
