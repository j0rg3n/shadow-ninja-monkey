/**
 * ...
 * @author jorgen@fabeljet.com
 */

//package ;

import flash.display.MovieClip;
import flash.display.SimpleButton;
import flash.events.DataEvent;
import flash.events.Event;
import flash.events.ProgressEvent;
import flash.net.Socket;
import flash.text.TextField;
import flash.text.TextFieldType;

class Test 
{
	static function main() 
	{
        //trace("Hello World !");		

		instance = new Test(flash.Lib.current);
	}
	
	public function new(clip : MovieClip)
	{
		//clip.addEventListener(Event.ENTER_FRAME, onFrame, false, 0, true);
		serverAddressTextField = new TextField();
		serverAddressTextField.text = "punch.fabeljet.com";
		serverAddressTextField.type = TextFieldType.INPUT;
		//serverAddressTextField.restrict = "0-9.";
		
		connect = new SimpleButton();
		
		//clip.addChild(serverAddressTextField);
		//clip.addChild(connect);
		
		var socket : Socket = new Socket();
		socket.addEventListener(Event.CONNECT, onConnect);
		socket.addEventListener(ProgressEvent.SOCKET_DATA, onData);
		socket.addEventListener(Event.CLOSE, onClose);
		
		var hostName : String = "fabeljet.com";
		socket.connect(hostName, 80);
		socket.writeUTF("GET /punch/index.php http/1.1\r\n");
		socket.writeUTF("Host: " + hostName + "\r\n\r\n");
		socket.flush();
	}
	
	private function onData(arg : ProgressEvent)
	{
		var socket : Socket = arg.target;
		trace("data received: " + socket.readUTFBytes(arg.bytesLoaded));
	}
	
	private function onConnect(arg : Dynamic)
	{
		trace("connection opened.");
	}
	
	private function onClose(arg : Dynamic)
	{
		trace("connection closed.");
	}
	
	private static function onFrame(param : Dynamic)
	{
		++i;
		
		var mc:flash.display.MovieClip = flash.Lib.current;
		/*
        mc.graphics.beginFill(0xFF0000);
        mc.graphics.moveTo(50,50 + i);
        mc.graphics.lineTo(100,50 + i);
        mc.graphics.lineTo(100,100 + i);
        mc.graphics.lineTo(50,100 + i);
        mc.graphics.endFill();
		*/
	}

	static var instance : Test;
	static var i : Int = 0;
	
	var clip : flash.display.MovieClip;
	var serverAddressTextField : TextField;
	var connect : SimpleButton;
}