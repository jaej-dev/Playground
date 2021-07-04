

// Receiver
public class Fan {
	public static final int HIGH = 2;
	public static final int LOW = 1;
	public static final int OFF = 0;
	String location;
	int speed;
	
	public Fan(String location) {
		this.location = location;
		speed = OFF;
	}
	
	public void high() {
		speed = HIGH;
		System.out.println(location + " fan is on high");
	}
	
	public void low() {
		speed = LOW;
		System.out.println(location + " fan is on low");
	}
	
	public void off() {
		speed = HIGH;
		System.out.println(location + " fan is off");
	}
	
	public int getSpeed() {
		return speed;
	}
}