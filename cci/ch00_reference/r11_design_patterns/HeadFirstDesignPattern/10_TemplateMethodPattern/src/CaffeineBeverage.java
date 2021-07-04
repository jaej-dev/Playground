

// Abstract class
public abstract class CaffeineBeverage {
 
	// Template method
	final void prepareRecipe() {
		boilWater();
		brew();
		pourInCup();
		if (hookCustomerWantsCondiments()) {
			addCondiments();
		}
	}
 
	// Primitive operation
	abstract void brew();
 	abstract void addCondiments();
 
	// Internal algorithm phases 
	void boilWater() {
		System.out.println("Boiling water");
	}
 	void pourInCup() {
		System.out.println("Pouring into cup");
	}
 
	// Hook
	boolean hookCustomerWantsCondiments() {
		return true;
	}
}