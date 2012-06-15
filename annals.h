#ifndef annals_h
#define annals_h

#include "string.h"
#include "filesystem.h"

/*
The verbosity levels are at no output (level 9) to quite verbose (level 1).  Not
all levels may be used.  Every message at a higer level than the current
logging level are recorded.

Create a sub-class of Ledger to use custom output methods (like OpenGL).  If you
set an extra ledger, all log events will instead be routed to the extra ledger.
*/

//
enum LedgerLevels
{
	rlVerbose = 1,
	rlDefault = 5,
	rlWarnings = 6,
	rlErrors = 7,
	rlFatalErrors = 8,
	rlOff = 9
};

class Ledger
{
	public:
		virtual ~Ledger(void) {}
		virtual void HandleLog(int Level, const String &Message, const String &Extra) = 0;
};

// Core class, required for settings and init/destruct
class AnnalsBase
{
	public:
		AnnalsBase(FilePath const &FileOutputLocation);
		~AnnalsBase(void);

		void SetExtraLedger(Ledger *ExtraLedger);

		void DefaultSettings(void);
		void SetFileOutput(bool On, int MinimumLevel = 5);
		void SetFileOutputLocation(FilePath const &Location);
		void SetConsoleOutput(bool On = true, int MinimumLevel = 6);
		//void SetScreenOutput(bool On = false, int MinimumLevel = 4);

		// Main logging function
		void Log(int Level, const String &Message, String Extra = String());

		// Convenience functions
		void Notify(const String &Message, const String &Extra = String());
		void Declare(const String &Message);
		void Warn(const String &Message, const String &Extra = String());
		void Error(const String &Message, const String &Extra = String());
		void Fatal(const String &Message, const String &Extra = String());

	private:
		int FileLevel, ConsoleLevel;

		FilePath DefaultLocation;
		FileOutput FileOutputInstance;

		Ledger *ExtraLedger;
};

AnnalsBase &GeneralAnnals(void);

#endif
