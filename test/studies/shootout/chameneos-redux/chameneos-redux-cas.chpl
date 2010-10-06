_extern type volatileint32 = uint(32); // uintptr_t volatile
_extern def __sync_val_compare_and_swap_c(inout state_p : volatileint32, state : volatileint32, xchg : volatileint32) : volatileint32;
_extern def sched_yield();

use Time;

/*	- The Chameneos game is as follows: 
	  	A population of n chameneos gathers at a common meeting place, where 
	  	m meetings will take place (n and m may be distinct).  At any time, only one
		meeting may take place between exactly two chameneos, and each chameneos may be 
		either blue, red, or yellow.  During a meeting, the chameneos exchange information 
		about the other chameneos' color, so that after the meeting it can change its own 
		color to the complement of its original color and the other chameneos' color.  (The 
		complement is respective to two colors, its own and its partner's, such that if 
		both colors are the same, no change, otherwise each chameneos changes to the color 
		you and your partner both are not.)	 

	- (description of benchmark: http://shootout.alioth.debian.org/u32q/benchmark.php?test=chameneosredux&lang=all */

config const numMeetings : int = 6000000;	// number of meetings to take place
config const spinCount : int = 20000;		// time to spin before calling sched_yield()	
config const numChameneos1 : int = 3;		// size of population 1
config const numChameneos2 : int = 10;  	// size of population 2
enum Color {blue=0, red=1, yellow=2}; 	 
enum Digit {zero, one, two, three, four, 	
	     five, six, seven, eight, nine};
config const verbose = false;			// if verbose is true, prints out non-det output, otherwise prints det output
config const peek = false;			// if peek is true, allows chameneos to peek at spotsLeft$ in MeetingPlace.meet()
						// then immediately return
config const CHAMENEOS_IDX_MASK = 0xFF;
config const MEET_COUNT_SHIFT = 8;


class MeetingPlace {
	var state : volatileint32;

	/* constructor for MeetingPlace, sets the 
	   number of meetings to take place */
	def MeetingPlace() {
		state = (numMeetings << MEET_COUNT_SHIFT) : uint;
	}
	
	/* reset must be called after meet, 
	   to reset numMeetings for a subsequent call of meet */
	def reset() {
		state = (numMeetings << MEET_COUNT_SHIFT) : uint;
	}
}
	


/* getComplement returns the complement of this and another color:
   if this and the other color are of the same value, return own value
   otherwise return the color that is neither this or the other color */
def getComplement(myColor : Color, otherColor : Color) {
	if (myColor == otherColor) { return myColor; } 
	return (3 - myColor - otherColor) : Color;
}

class Chameneos {
	var id: int;
	var color : Color;
	var meetings : int;
	var meetingsWithSelf : int;
	var meetingCompleted : volatileint32;
	
	/* start tells a Chameneos to go to a given MeetingPlace, where it may meet 
	   with another Chameneos.  If it does, it will get the complement of the color
	   of the Chameneos it met with, and change to the complement of that color. */
	def start(population : [] Chameneos, inout state : volatileint32) {
		var stateTemp : uint;
		var peer : Chameneos;
		var peer_idx : uint;	
		var xchg : uint;
		var prev : uint;
		var is_same : int;
		var newColor : Color;

		//writeln("id ", id, ": in start");
		stateTemp = state;	
					
		while (true) {
			peer_idx = stateTemp & CHAMENEOS_IDX_MASK;
			writeln("id ", id, ": peer_idx is ", peer_idx);
			if (peer_idx) {
				xchg = stateTemp - peer_idx - (1 << MEET_COUNT_SHIFT):uint;
			} else if (stateTemp) {
				xchg = stateTemp | id;		
			} else {
				break;
			}
			//writeln("id ", id, ": xchg is ", xchg);
			prev = __sync_val_compare_and_swap_c(state, stateTemp, xchg);
			if (prev == stateTemp) {
				if (peer_idx) {
					//writeln("id ", id, ": got here second");
					if (id == peer_idx) {
						is_same = 1;
						halt("halt: chameneos met with self");
					}
					peer = population[peer_idx:int];
					newColor = getComplement(color, peer.color);
					peer.color = newColor;
					peer.meetings += 1;
					peer.meetingsWithSelf += is_same;
					peer.meetingCompleted = 1;
					color = newColor;
					meetings += 1;
					meetingsWithSelf += is_same;
					//writeln("id ", id, ": exiting");

				} else {
					//writeln("id ", id, ": got here first");
					while (meetingCompleted == 0) {
						sched_yield();
					}
					//writeln("id ", id, ": exiting");
					meetingCompleted = 0;
					stateTemp = state;	
				}
			} else {
				stateTemp = prev; 
			}
		} 
	}
	
}

/* printColorChanges prints the result of getComplement for all possible pairs of colors */
def printColorChanges() {
	const colors : [1..3] Color = (Color.blue, Color.red, Color.yellow);
	for color1 in colors {
		for color2 in colors {
			writeln(color1, " + ", color2, " -> ", getComplement(color1, color2));
		}
	}
	writeln();
}

/* populate takes an parameter of type int, size, and returns a population of chameneos 
   of that size. if population size is set to 10, will use preset array of colors  */
def populate (size : int) {
	const colorsDefault10  = (Color.blue, Color.red, Color.yellow, Color.red, Color.yellow, 
			      	        Color.blue, Color.red, Color.yellow, Color.red, Color.blue);	
	const D : domain(1) = [1..size];
	var population : [D] Chameneos;

	if (size == 10) {
		for i in D {population(i) = new Chameneos(i, colorsDefault10(i));}	
	} else { 
		for i in D {population(i) = new Chameneos(i, ((i-1) % 3):Color);}
	}
	return population;
}

/* run takes a population of Chameneos and a MeetingPlace, then allows the Chameneos to meet.
   it then prints out the number of times each Chameneos met another Chameneos, spells out the
   number of times it met with itself, then spells out the total number of times all the Chameneos met
   another Chameneos. */
def run(population : [] Chameneos, meetingPlace : MeetingPlace) {
	for i in population { write(" ", i.color); }
	writeln();

	coforall i in population { i.start(population, meetingPlace.state); }
	meetingPlace.reset();
}

def runQuiet(population : [] Chameneos, meetingPlace : MeetingPlace) {
	coforall i in population { i.start(population, meetingPlace.state); }
	meetingPlace.reset();

	const totalMeetings = + reduce population.meetings;
	const totalMeetingsWithSelf = + reduce population.meetingsWithSelf;
	if (totalMeetings == numMeetings*2) { writeln("total meetings PASS"); } 
	else { writeln("total meetings actual = ", totalMeetings, ", total meetings expected = ", numMeetings*2);}

	if (totalMeetingsWithSelf == 0) { writeln("total meetings with self PASS"); }
	else { writeln("total meetings with self actual = ", totalMeetingsWithSelf, ", total meetings with self expected = 0");}

	writeln();
}

def printInfo(population : [] Chameneos) {
	for i in population {
		write(i.meetings);
		spellInt(i.meetingsWithSelf);
	}
	const totalMeetings = + reduce population.meetings;
	spellInt(totalMeetings);
	writeln();
}

/* spellInt takes an integer, and spells each of its digits out in English */
def spellInt(n : int) {
	var s : string = n:string;
	for i in 1..s.length {write(" ", (s.substring(i):int + 1):Digit);}
	writeln();
}

def main() {
	if (numChameneos1 < 2 || numChameneos2 < 2 || numMeetings < 0) {
		writeln("Please specify numChameneos1 and numChameneos2 of at least 2, and numMeetings of at least 0.");
	} else 	{	
		var start_time = getCurrentTime();
		
		printColorChanges();	

		const forest : MeetingPlace = new MeetingPlace();	

		const population1 = populate(numChameneos1);
		const population2 = populate(numChameneos2);
		
		if (verbose) {
			var startTime1 = getCurrentTime();
			run(population1, forest);
			var endTime1 = getCurrentTime();
			writeln("time for chameneos1 to meet = ", endTime1 - startTime1);	
			printInfo(population1);

			var startTime2 = getCurrentTime();
			run(population2, forest);
			var endTime2 = getCurrentTime();
			writeln("time for chameneos2 to meet = ", endTime2 - startTime2);
			printInfo(population2);
		} else {
			runQuiet(population1, forest);
			runQuiet(population2, forest);
		}
		var end_time = getCurrentTime();
		if (verbose) {
			writeln("total execution time = ", end_time - start_time);
		}
	}
}

