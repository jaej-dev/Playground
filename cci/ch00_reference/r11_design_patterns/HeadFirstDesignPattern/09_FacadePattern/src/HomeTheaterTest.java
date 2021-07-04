

// Client
public class HomeTheaterTest {
	public static void main(String[] args) {
		Amplifier amp = new Amplifier("AMPLIFIER");
		RadioTuner tuner = new RadioTuner("RADIO_TUNER", amp);
		DvdPlayer dvd = new DvdPlayer("DVD_PLAYER", amp);
		CdPlayer cd = new CdPlayer("CD_PLAYER", amp);
		Projector projector = new Projector("PROJECTOR", dvd);
		TheaterLights lights = new TheaterLights("THEATER_LIGHTS");
		Screen screen = new Screen("THEATER_SCREEN");
		PopcornPopper popper = new PopcornPopper("POPCORN_POPPER");
 
		HomeTheaterFacade homeTheater = 
				new HomeTheaterFacade(amp, tuner, dvd, cd, 
						projector, screen, lights, popper);
 
		homeTheater.watchMovie("BEN-HUR");
		homeTheater.endMovie();
		
		homeTheater.listenToRadio(99.9f);
		homeTheater.endRadio();
		
		homeTheater.listenToCd("THE PIANO PLAYER");
		homeTheater.endCd();
	}
}