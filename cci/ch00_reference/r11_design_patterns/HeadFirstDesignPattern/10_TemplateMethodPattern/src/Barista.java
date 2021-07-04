

public class Barista {
	public static void main(String[] args) {
 		
		Coffee coffee = new Coffee();
		Tea tea = new Tea();
 
		System.out.println("\n# Making coffee...");
		coffee.prepareRecipe();
		
		System.out.println("\n# Making tea...");
		tea.prepareRecipe();
	}
}