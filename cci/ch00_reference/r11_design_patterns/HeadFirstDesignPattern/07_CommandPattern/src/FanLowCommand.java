

public class FanLowCommand implements Command {
	Fan fan;
	int prevSpeed;
	
	public FanLowCommand(Fan fan) {
		this.fan = fan;
	}
	
	public void execute() {
		prevSpeed = fan.getSpeed();
		fan.low();
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