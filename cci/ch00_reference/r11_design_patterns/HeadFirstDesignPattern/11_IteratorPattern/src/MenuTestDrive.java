

public class MenuTestDrive {
	public static void main(String args[]) {
		PancakeHouseMenu pancakeHouseMenu = new PancakeHouseMenu();
		DinerMenu dinerMenu = new DinerMenu();
		CafeMenu cafeMenu = new CafeMenu();
 
		Waitress waitress = new Waitress(pancakeHouseMenu, dinerMenu, cafeMenu);
 
		waitress.printMenu();
		waitress.printVegetarianMenu();

		System.out.println("\n> Customer asks: Is the Hotdog vegetarian?");
		System.out.print("> Waitress says: ");
		if (waitress.isItemVegetarian("Hotdog")) {
			System.out.println("Yes");
		} else {
			System.out.println("No");
		}
		System.out.println("\n> Customer asks: Are the Waffles vegetarian?");
		System.out.print("> Waitress says: ");
		if (waitress.isItemVegetarian("Waffles")) {
			System.out.println("Yes");
		} else {
			System.out.println("No");
		}
	}
}