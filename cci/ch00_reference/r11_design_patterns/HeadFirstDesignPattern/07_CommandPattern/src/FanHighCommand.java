

public class FanHighCommand implements Command {
	Fan fan;
	int prevSpeed;
	
	public FanHighCommand(Fan fan) {
		this.fan = fan;
	}
	
	public void execute() {
		prevSpeed = fan.getSpeed();
		fan.high();
	}
	
	public void undo() {
		if (prevSpeed == Fan.HIGH) {
			fan.high();
		} else if (prevSpeed == Fan.LOW) {
			fan.low();
		} else if (prevSpeed == Fan.OFF) {
			fan.off();
		}
	}
}