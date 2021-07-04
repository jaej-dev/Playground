

import java.util.Iterator;
  
public class Waitress {
	Menu pancakeHouseMenu;
	Menu dinerMenu;
	Menu cafeMenu;
 
	public Waitress(Menu pancakeHouseMenu, Menu dinerMenu, Menu cafeMenu) {
		this.pancakeHouseMenu = pancakeHouseMenu;
		this.dinerMenu = dinerMenu;
		this.cafeMenu = cafeMenu;
	}
 
	public void printMenu() {
		Iterator<MenuItem> pancakeIterator = pancakeHouseMenu.createIterator();
		Iterator<MenuItem> dinerIterator = dinerMenu.createIterator();
		Iterator<MenuItem> cafeIterator = cafeMenu.createIterator();

		System.out.println("### MENU ###\n\n# PANCAKE MENU");
		printMenu(pancakeIterator);
		System.out.println("\n# DINER MENU");
		printMenu(dinerIterator);
		System.out.println("\n# CAFE MENU");
		printMenu(cafeIterator);
	}
 
	private void printMenu(Iterator<MenuItem> iterator) {
		while (iterator.hasNext()) {
			MenuItem menuItem = iterator.next();
			System.out.print(menuItem.getName() + ", ");
			System.out.print(menuItem.getPrice() + " -- ");
			System.out.println(menuItem.getDescription());
		}
	}
 
	public void printVegetarianMenu() {
		System.out.println("\n# VEGETARIAN PANCAKE MENU");
		printVegetarianMenu(pancakeHouseMenu.createIterator());
		System.out.println("\n# VEGETARIAN DINER MENU");
		printVegetarianMenu(dinerMenu.createIterator());
		System.out.println("\n# VEGETARIAN CAFE MENU");
		printVegetarianMenu(cafeMenu.createIterator());
	}
 
	public boolean isItemVegetarian(String name) {
		Iterator<MenuItem> pancakeIterator = pancakeHouseMenu.createIterator();
		if (isVegetarian(name, pancakeIterator)) {
			return true;
		}
		Iterator<MenuItem> dinerIterator = dinerMenu.createIterator();
		if (isVegetarian(name, dinerIterator)) {
			return true;
		}
		Iterator<MenuItem> cafeIterator = cafeMenu.createIterator();
		if (isVegetarian(name, cafeIterator)) {
			return true;
		}
		return false;
	}


	private void printVegetarianMenu(Iterator<MenuItem> iterator) {
		while (iterator.hasNext()) {
			MenuItem menuItem = iterator.next();
			if (menuItem.isVegetarian()) {
				System.out.print(menuItem.getName() + ", ");
				System.out.print(menuItem.getPrice() + " -- ");
				System.out.println(menuItem.getDescription());
			}
		}
	}

	private boolean isVegetarian(String name, Iterator<MenuItem> iterator) {
		while (iterator.hasNext()) {
			MenuItem menuItem = iterator.next();
			if (menuItem.getName().equals(name)) {
				if (menuItem.isVegetarian()) {
					return true;
				}
			}
		}
		return false;
	}
}