#include "annals.h"

#include "string.h"

#include <iostream>
#include <stdlib.h>

AnnalsBase::AnnalsBase(const String &FileOutputLocation) :
	FileLevel(rlDefault), ConsoleLevel(rlWarnings),
	DefaultFilename(FileOutputLocation), FileOutput(new OutputStream(FileOutputLocation.c_str())),
	ExtraLedger(NULL)
	{}

AnnalsBase::~AnnalsBase(void)
{
	delete FileOutput;
}

void AnnalsBase::SetExtraLedger(Ledger *NewLedger)
{
	ExtraLedger = NewLedger;
}

void AnnalsBase::DefaultSettings(void)
{
	FileLevel = rlDefault;
	ConsoleLevel = rlWarnings;
	delete FileOutput;
	FileOutput = new OutputStream(DefaultFilename.c_str(), OutputStream::app);
}

void AnnalsBase::SetFileOutput(bool On, int MinimumLevel)
{
	if (!On) FileLevel = rlOff;
	else FileLevel = MinimumLevel;
}

void AnnalsBase::SetFileOutputLocation(const String &Location)
{
	delete FileOutput;
	FileOutput = new OutputStream(Location.c_str(), OutputStream::app | OutputStream::trunc);
}

void AnnalsBase::SetConsoleOutput(bool On, int MinimumLevel)
{
	if (!On) ConsoleLevel = rlOff;
	else ConsoleLevel = MinimumLevel;
}

// Main logging function
void AnnalsBase::Log(int Level, const String &Message, String Extra)
{
	if (ExtraLedger != NULL) ExtraLedger->HandleLog(Level, Message, Extra);

	if ((ConsoleLevel == rlOff) && (FileLevel == rlOff)) return;

	// Render the output string
	//time_t GlobalTime = time(NULL);
	//String Timestamp = asctime(localtime(&GlobalTime));
	//Timestamp = Timestamp.erase(Timestamp.find("\n"));

	StringStream Rendered;
	//Rendered << Timestamp << ", ";

	if (Level == rlFatalErrors) Rendered << "Fatal Error ";
	else if (Level == rlErrors) Rendered << "Error ";
	else if (Level == rlWarnings) Rendered << "Warning ";

	Rendered << "Level " << Level << ": " << Message << "\n";

	if (!Extra.empty())
	{
		String::size_type LastFind = 0;
		while ((LastFind = Extra.find("\n", LastFind + 1)) != String::npos)
			Extra.insert(LastFind + 1, "\t");
		Rendered << "\t" << Extra << "\n";
	}

	if (Level >= FileLevel) *FileOutput << Rendered.str() << std::flush;
	if (Level >= ConsoleLevel) std::cout << Rendered.str() << std::flush;
}

// Convenience functions
void AnnalsBase::Notify(const String &Message, const String &Extra)
	{ Log(3, Message, Extra); }

void AnnalsBase::Declare(const String &Message)
	{ Log(5, Message); }

void AnnalsBase::Warn(const String &Message, const String &Extra)
	{ Log(6, Message, Extra); }

void AnnalsBase::Error(const String &Message, const String &Extra)
	{ Log(7, Message, Extra); }

void AnnalsBase::Fatal(const String &Message, const String &Extra)
	{ Log(8, Message, Extra); exit(1); }

static AnnalsBase *GeneralAnnalsInstance(NULL);

AnnalsBase &GeneralAnnals(void)
{
	if (GeneralAnnalsInstance == NULL)
		GeneralAnnalsInstance = new AnnalsBase("generallog.txt");
	return *GeneralAnnalsInstance;
}
