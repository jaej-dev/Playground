//package headfirst.designpatterns.strategy;


public class MiniDuckSimulator {
	public static void main(String[] args) {
	    MallardDuck mallard = new MallardDuck();
	    RubberDuck rubberDuckie = new RubberDuck();
	    DecoyDuck decoy = new DecoyDuck();
	
	    Duck model = new ModelDuck();
	
	    System.out.println("-- Mallard duck --");
	    mallard.performQuack();
	    mallard.performFly();
	    
	    System.out.println("-- Rubber duck --");
	    rubberDuckie.performQuack();
	    rubberDuckie.performFly();
	    
	    System.out.println("-- Decoy duck --");
	    decoy.performQuack();
	    decoy.performFly();
	
	    System.out.println("-- Model duck --");
	    model.performQuack();
	    model.setQuackBehavior(new Squeak());
	    model.performQuack();
	    model.performFly();
	    model.setFlyBehavior(new FlyRocketPowered());
	    model.performFly();
	}
}