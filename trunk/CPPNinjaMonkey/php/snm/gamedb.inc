<?php
require_once('debug.inc');
require_once('util.inc');
//require_once('piece.inc');

class GameDB
{
    public function GameDB()
    {
        $this->openConnection();
    }
    
    public function destroy()
    {
        $this->closeConnection();
    }
	
	/**
	 * @param array $props
	 */
	public function handleHelloWorld($props)
	{
		return array('message' => sprintf('Hello, %s!', $props['name']));
	}
    
    /**
     * @param array $pieces
     */
    public function handleCommitBoard($pieces)
    {
        $this->updatePieces($pieces);
        $this->commit();
        return $this->getPieces();
    }
    
    public function handlePoll()
    {
        // TODO: Don't open the transaction at all when polling.
        $this->commit();
        
        for ($i = 0; $i < 20; ++$i)
        {
            // TODO: Do poll of values/latest revision in DB. DB must contain log of the game.
            echo ' ';
            sleep(25);
        }
        return 'Polled.';
    }

    public function getGameID()
    {
        return $this->gameID;
    }
    
    public function getPlayerID()
    {
        return $this->playerID;
    }
    
    public function getMagic()
    {
        return $this->magic;
    }
    
    public function getPieces()
    {
        return "";
    }
    
    public function commit()
    {
        $this->queryOrDie('COMMIT', 'commit transaction');
    }
    
    public function initPlayerSession()
    {
        session_start();
        if (array_key_exists('playerID', $_SESSION))
        {
            $this->playerID = $_SESSION['playerID'];
        }
        else
        {
            $this->gameID = $this->getRunningGame();
            $_SESSION['playerID'] = $this->playerID = $this->createPlayer();
            //$this->addPiece(PieceFactory::GetPiece($this->playerID, 'Player', null, null, PLAYER_STATE_NOT_STARTED));
            $this->commit();
        }
        session_write_close();
        
        $this->loadPlayer();
        //$this->loadPieces();
    }
    
    public function loadPlayer()
    {
        $result = $this->queryOrDie('SELECT `magic`, `game_id` FROM `player` WHERE `id`=' . $this->playerID, 'running games');
        list($this->magic, $this->gameID) = mysql_fetch_row($result);
        mysql_free_result($result);
    }
    
    private function createPlayer()
    {
        $insert = 'INSERT INTO `player` (`game_id`, `magic`) VALUES (' . $this->gameID . ', ' . '\'' . Util::getMagic(16) . '\')';
        $this->queryOrDie($insert, 'create new player');
        return mysql_insert_id($this->connection);
    }
    
    /**
     * @desc Get existing running game, or create a new one if none exists.
     */
    private function getRunningGame()
    {
        // Find old game
        // TODO: Count games older than, say, 2 hours, as ended.
        $result = $this->queryOrDie('SELECT `id` FROM `game` WHERE `end` IS NULL', 'running games');
        $row = mysql_fetch_row($result);
        mysql_free_result($result);
        if ($row)
        {
            return $row[0];
        }
        
        // Create new game
        $this->queryOrDie('INSERT INTO `game` (`start`) VALUES (NOW())', 'create new game');
        return mysql_insert_id($this->connection);
    }
    
    private function queryOrDie($query, $objective)
    {
        $result = mysql_query($query, $this->connection);
        if ($result == null)
        {
            $message = 'Failed sql query for "' . $objective . '"';
            if (DEBUG)
            {
                $message .= ': "' . htmlspecialchars($query) . '"';
                $message .= ': ' . htmlspecialchars(mysql_error($this->connection));
            }
            die($message);
        }
        return $result;
    }

    private function openConnection()
    {
        register_shutdown_function(array($this, 'destroy'));

        $password = trim(file_get_contents('secret.txt'));
        $this->connection = mysql_connect('localhost', 'fabeljet_com', $password);
        if ($this->connection == null)
        {
            die('Could not connect to mysql server.');
        }

        mysql_select_db('fabeljet_com', $this->connection) || die('Could not select database.');
        
        $this->queryOrDie('START TRANSACTION', 'start transaction');
    }

    private function closeConnection()
    {
        if ($this->connection)
        {
            mysql_close($this->connection);
        }
    }

    private function getSQLInt($value)
    {
        return $value === null ? 'NULL' : strval($value);
    }
    
    private function getSQLString($value)
    {
        return $value === null ? 'NULL' : '\'' . mysql_real_escape_string($value, $this->connection) . '\'';
    }

    private $gameID;
    private $playerID;
    private $magic;
    private $connection;
}
?>
