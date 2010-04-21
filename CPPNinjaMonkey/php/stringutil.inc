<?php
class StringUtil
{
    public static function endsWith($string, $postfix)
    {
        $postfixStart = strlen($string) - strlen($postfix);
        return $postfixStart >= 0 && substr($string, $postfixStart) == $postfix;
    }
}  
?>
