#include <iostream>
#include <string>
using namespace std;

//Your declarations and implementations would go here

class Event
{
	public:
		Event(string name) { m_name = name; }
		virtual ~Event() {}
		virtual string need() const = 0;
		virtual bool isSport() const { return false; }
		string name() const { return m_name; }
	private:
		string m_name;
};

class BasketballGame : public Event
{
	public:
		BasketballGame(string name) : Event(name){}
		virtual ~BasketballGame()
		{
			cout << "Destroying the " << name() << " basketball game" << endl;
			//delete this;
		}
		virtual string need() const { return "hoops"; }
		virtual bool isSport() const { return true; }
};

class HockeyGame : public Event
{
	public:
		HockeyGame(string name) : Event(name) {}
		virtual ~HockeyGame()
		{
			cout << "Destroying the " << name() << " hockey game" << endl;
			//delete this;
		}
		virtual string need() const { return "ice"; }
		virtual bool isSport() const { return true; }
};

class Concert : public Event
{
	public:
		Concert(string bandName, string genre) : Event(bandName) { m_genre = genre; }
		virtual ~Concert()
		{
			cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
			//delete this;
		}
		virtual string need() const { return "a stage"; }
	private:
		string m_genre;
};

void display(const Event* e)
{
	cout << e->name() << ": ";
	if (e->isSport())
		cout << "(sport) ";
	cout << "needs " << e->need() << endl;
}

int main()
{
	Event* events[4];
	events[0] = new BasketballGame("Lakers vs. Suns");
	// Concerts have a name and a genre.
	events[1] = new Concert("Banda MS", "banda");
	events[2] = new Concert("KISS", "hard rock");
	events[3] = new HockeyGame("Kings vs. Flames");

	cout << "Here are the events." << endl;
	for (int k = 0; k < 4; k++)
		display(events[k]);

	// Clean up the events before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete events[k];
}