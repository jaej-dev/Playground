

// Client
public class RemoteControlTest{
	
	public static void main(String[] args) {
		RemoteControl rc = new RemoteControl();
		Light roomLight = new Light("Room");
		LightOnCommand roomLightOnCmd = new LightOnCommand(roomLight);
		LightOffCommand roomLightOffCmd = new LightOffCommand(roomLight);
		
		rc.setCommand(0, roomLightOnCmd, roomLightOffCmd);

		System.out.println("\n< Test 1: Light on -> off -> undo >\n");
		rc.onButtonWasPushed(0);
		rc.offButtonWasPushed(0);
		System.out.println(rc);
		rc.undoButtonWasPushed();

		System.out.println("\n< Test 2: Light off -> on -> undo > \n");
		rc.offButtonWasPushed(0);
		rc.onButtonWasPushed(0);
		System.out.println(rc);
		rc.undoButtonWasPushed();
		
		Fan garageFan = new Fan("Garage");
		FanHighCommand grageFanHighCmd = new FanHighCommand(garageFan);
		FanLowCommand grageFanLowCmd = new FanLowCommand(garageFan);
		FanOffCommand grageFanOffCmd = new FanOffCommand(garageFan);
		
		rc.setCommand(0, grageFanHighCmd, grageFanOffCmd);
		rc.setCommand(1, grageFanLowCmd, grageFanOffCmd);
		
		System.out.println("\n< Test 3: Fan high -> off -> undo > \n");
		rc.onButtonWasPushed(0);
		rc.offButtonWasPushed(0);
		System.out.println(rc);
		rc.undoButtonWasPushed();
		
		System.out.println("\n< Test 4: Fan low -> off -> undo > \n");
		rc.onButtonWasPushed(1);
		rc.offButtonWasPushed(1);
		System.out.println(rc);
		rc.undoButtonWasPushed();
	}
}