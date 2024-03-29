<?php
/**
 * AJAX entry point.
 */
require_once('debug.inc');
require_once('util.inc');
require_once('gamedb.inc');

function getDispatchTable()
{
    global $db;
    return array(
        'commitBoard' => array($db, 'handleCommitBoard'),
        'poll' => array($db, 'handlePoll'),
        'helloWorld' => array($db, 'handleHelloWorld'),
    );
}

// TODO: Consider using the raw request body to 
// provide your very own RPC call encoding.
//$requestBody = file_get_contents("php://input");

$db = new GameDB();
$db->initPlayerSession();

$dispatchTable = getDispatchTable();
$functionName = Util::getParameter('name');
if (!array_key_exists($functionName, $dispatchTable))
{
    die('Unknown request: "' . htmlspecialchars($functionName) . '"');
}

$encodedArgs = Util::getParameter('args');
$args = json_decode($encodedArgs, true);
if ($args === null) 
{
    die('Failed parsing: "' . $encodedArgs . '"');
}

$result = call_user_func($dispatchTable[$functionName], $args);
echo json_encode($result);
?>
