#include "annals.h"

#include "string.h"

#include <iostream>
#include <stdlib.h>

AnnalsBase::AnnalsBase(FilePath const &FileOutputLocation) :
	FileLevel(rlDefault), ConsoleLevel(rlWarnings),
	DefaultLocation(FileOutputLocation), FileOutputInstance(FileOutputLocation.AsAbsoluteString()), // TODO Use move constructor when libstdc++ supports it
	ExtraLedger(nullptr)
	{}

AnnalsBase::~AnnalsBase(void) { } 

void AnnalsBase::SetExtraLedger(Ledger *NewLedger)
{
	ExtraLedger = NewLedger;
}

void AnnalsBase::DefaultSettings(void)
{
	FileLevel = rlDefault;
	ConsoleLevel = rlWarnings;
	//FileOutputInstance = DefaultLocation.Write(true); // TODO Use move assignment when libstdc++ supports it
	FileOutputInstance.close();
	FileOutputInstance.open(DefaultLocation.AsAbsoluteString(), FileOutput::app);
}

void AnnalsBase::SetFileOutput(bool On, int MinimumLevel)
{
	if (!On) FileLevel = rlOff;
	else FileLevel = MinimumLevel;
}

void AnnalsBase::SetFileOutputLocation(FilePath const &Location)
{ 
	//FileOutputInstance = Location.Write(true, true); // TODO Use move assignment when libstdc++ supports it
	FileOutputInstance.close();
	FileOutputInstance.open(DefaultLocation.AsAbsoluteString(), FileOutput::app | FileOutput::trunc);
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

	if (Level >= FileLevel) FileOutputInstance << Rendered.str() << std::flush;
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
		GeneralAnnalsInstance = new AnnalsBase(LocateWorkingDirectory().Select("generallog.txt"));
	return *GeneralAnnalsInstance;
}
