

// Danger!  This implementation of Singleton not
// guaranteed to work prior to Java 5
//

public class Singleton {
	private volatile static Singleton uniqueInstance;
 
	private Singleton() {
		System.out.println("Created singleton instance...");
	}
 
	public static Singleton getInstance() {
		if (uniqueInstance == null) {
			synchronized (Singleton.class) {
				if (uniqueInstance == null) {
					uniqueInstance = new Singleton();
				}
			}
		}
		System.out.println("getInstance(): uniqeInstance = " + uniqueInstance);
		return uniqueInstance;
	}
}