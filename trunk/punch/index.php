<?php 

function closeDB()
{
	global $connection;
	mysql_close($connection);
}

function queryOrDie($query)
{
	global $connection;
	
	$result = mysql_query($query, $connection);
	if ($result == null)
	{
		$message = 'Failed sql query';
		$message .= ': "' . htmlspecialchars($query) . '"';
		$message .= ': ' . htmlspecialchars(mysql_error($connection));
		die($message);
	}
	
	return $result;
}

$sessionID = $_GET['SessionID'];
$clientID = $_GET['ClientID'];
$otherClientID = $clientID == 'A' ? 'B' : 'A';
$privateEndPoint = $_GET['PrivateEndPoint'];
$publicEndPoint = $_SERVER['REMOTE_ADDR'] . ':' . $_SERVER['REMOTE_PORT'];

printf('%s %s %s %s' . "\r\n", $clientID, $sessionID, $privateEndPoint, $publicEndPoint);

register_shutdown_function('closeDB');

$password = trim(file_get_contents('../snm/secret.txt'));
$connection = mysql_connect('localhost', 'fabeljet_com', $password);
if ($connection == null)
{
	die('Could not connect to mysql server.');
}

mysql_select_db('fabeljet_com', $connection) || die('Could not select database.');

queryOrDie('UPDATE `p2p` ' . 
	'SET `PrivateEndPoint`=\'' . mysql_real_escape_string($privateEndPoint, $connection) . '\', ' . 
	'`PublicEndPoint`=\'' . mysql_real_escape_string($publicEndPoint, $connection) . '\', ' . 
	'`SessionID`=\'' . mysql_real_escape_string($sessionID, $connection) . '\' ' . 
	'WHERE `ClientID`=\'' . mysql_real_escape_string($clientID, $connection) . '\'');

$retries = 3;	
	
for ($i = 0; $i < $retries; ++$i)
{
	$result = queryOrDie('SELECT `SessionID`, `PrivateEndPoint`, `PublicEndPoint` ' . 
		'FROM `p2p` ' . 
		'WHERE `ClientID`=\'' . mysql_real_escape_string($otherClientID, $connection) . '\'');

	$row = mysql_fetch_row($result);
	mysql_free_result($result);
	if ($row)
	{
		$storedSessionID = $row[0];
		printf('%s %s %s %s' . "\r\n", $otherClientID, $storedSessionID, $row[1], $row[2]);
		
		if ($storedSessionID == $sessionID)
		{
			break;
		}
	}
	
	sleep(1);
}
?>