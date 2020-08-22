/***********************************************************************
* flogger-test.cpp - FLogger unit tests                                *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2020 Markus Gans                                           *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <memory>
#include <sstream>
#include <string>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <final/final.h>

//----------------------------------------------------------------------
// class FLoggerTest
//----------------------------------------------------------------------

class myLogger : public finalcut::FLog
{
  public:
    void info (const std::string& entry) override
    {
      output << " Info: " << entry << std::endl;
    }

    void warn (const std::string& entry) override
    {
      output << " Warn: " << entry << std::endl;
    }

    void error (const std::string& entry) override
    {
      output << "Error: " << entry << std::endl;
    }

    void debug (const std::string& entry) override
    {
      output << "Debug: " << entry << std::endl;
    }

    void flush() override
    {
      // An implementation is not required in this context
    }

    void setOutputStream (const std::ostream& os) override
    { output.rdbuf(os.rdbuf()); }

    void setLineEnding (LineEnding) override
    {
      // An implementation is not required in this context
    }

    void enableTimestamp() override
    {
      // An implementation is not required in this context
    }

    void disableTimestamp() override
    {
      // An implementation is not required in this context
    }

  private:
    // Data member
    std::ostream output{std::cerr.rdbuf()};
};

//----------------------------------------------------------------------
// class FLoggerTest
//----------------------------------------------------------------------

class FLoggerTest : public CPPUNIT_NS::TestFixture
{
  public:
    FLoggerTest()
    { }

  protected:
    void classNameTest();
    void defaultObjectTest();
    void lineEndingTest();
    void timestampTest();
    void fileTest();
    void applicationObjectTest();

  private:
    // Adds code needed to register the test suite
    CPPUNIT_TEST_SUITE (FLoggerTest);

    // Add a methods to the test suite
    CPPUNIT_TEST (classNameTest);
    CPPUNIT_TEST (defaultObjectTest);
    CPPUNIT_TEST (lineEndingTest);
    CPPUNIT_TEST (timestampTest);
    CPPUNIT_TEST (fileTest);
    CPPUNIT_TEST (applicationObjectTest);

    // End of test suite definition
    CPPUNIT_TEST_SUITE_END();
};

//----------------------------------------------------------------------
void FLoggerTest::classNameTest()
{
  finalcut::FLogger log;
  const finalcut::FString& classname = log.getClassName();
  CPPUNIT_ASSERT ( classname == "FLogger" );
}

//----------------------------------------------------------------------
void FLoggerTest::defaultObjectTest()
{
  finalcut::FLogger log{};
  std::ostringstream buf{};
  log.setOutputStream(buf);
  log << "Hello, World!" << std::flush;  // Default level is "Info"
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << "Hel" << "lo," << " Wor" << "ld!" << std::flush;  // Several parts
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << "Hello, World!" << std::endl;  // std::endl
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Hello, World!\n\r\n" );
  buf.str("");  // Clear buffer

  log << finalcut::FLog::Info << "Hello, World!" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << finalcut::FLog::Warn << "Hello, World!" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[WARNING] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << "Hello, World!" << std::flush;  // Last level was "Warn"
  CPPUNIT_ASSERT ( buf.str() == "[WARNING] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << finalcut::FLog::Error << "Hello, World!" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << "Hello, World!" << std::flush;  // Last level was "Error"
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << finalcut::FLog::Debug << "Hello, World!" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[DEBUG] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log << "Hello, World!" << std::flush;  // Last level was "Debug"
  CPPUNIT_ASSERT ( buf.str() == "[DEBUG] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  // Without stream
  log.info("Hello, World!");
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log.warn("Hello, World!");
  CPPUNIT_ASSERT ( buf.str() == "[WARNING] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log.error("Hello, World!");
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] Hello, World!\r\n" );
  buf.str("");  // Clear buffer

  log.debug("Hello, World!");
  CPPUNIT_ASSERT ( buf.str() == "[DEBUG] Hello, World!\r\n" );
  buf.str("");  // Clear buffer
}

//----------------------------------------------------------------------
void FLoggerTest::lineEndingTest()
{
  finalcut::FLogger log{};
  std::ostringstream buf{};
  log.setOutputStream(buf);

  log.info("Line endings");  // Default = CRLF
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Line endings\r\n" );
  buf.str("");  // Clear buffer

  log.setLineEnding(finalcut::FLog::LF);
  log.warn("Line endings");
  CPPUNIT_ASSERT ( buf.str() == "[WARNING] Line endings\n" );
  buf.str("");  // Clear buffer

  log.setLineEnding(finalcut::FLog::CR);
  log.error("Line endings");
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] Line endings\r" );
  buf.str("");  // Clear buffer

  log.setLineEnding(finalcut::FLog::CRLF);
  log.debug("Line endings");
  CPPUNIT_ASSERT ( buf.str() == "[DEBUG] Line endings\r\n" );
  buf.str("");  // Clear buffer
}

//----------------------------------------------------------------------
void FLoggerTest::timestampTest()
{
  finalcut::FLogger log{};
  std::ostringstream buf{};
  log.setOutputStream(buf);

  log.info("Timestamp");
  std::size_t length = buf.str().length();
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Timestamp\r\n" );
  CPPUNIT_ASSERT ( length == 18 );
  buf.str("");  // Clear buffer

  log.enableTimestamp();
  log.info("Timestamp");
  length = buf.str().length();
  CPPUNIT_ASSERT ( buf.str().substr(length - 18) == "[INFO] Timestamp\r\n" );
  CPPUNIT_ASSERT ( length > 40 );
  buf.str("");  // Clear buffer

  log.disableTimestamp();
  log.info("Timestamp");
  length = buf.str().length();
  CPPUNIT_ASSERT ( buf.str() == "[INFO] Timestamp\r\n" );
  CPPUNIT_ASSERT ( length == 18 );
  buf.str("");  // Clear buffer
}

//----------------------------------------------------------------------
void FLoggerTest::fileTest()
{
  std::string filename = "test.log";

  {
    finalcut::FLogger log{};
    std::ofstream file_stream(filename, std::ofstream::out);
    log.setLineEnding (finalcut::FLog::LF);
    log.setOutputStream(file_stream);

    log.info("test1");
    log.warn("test2");
    log.error("test3");
    log.debug("test4");
    log << finalcut::FLog::Info << "streaming test1";
    log << finalcut::FLog::Warn << "streaming test2";
    log << finalcut::FLog::Error << "streaming test3";
    log << finalcut::FLog::Debug << "streaming test4" << std::flush;

    if ( file_stream.is_open() )
      file_stream.close();
  }  // End of logging


  std::string strings[] =
  {
    "[INFO] test1",
    "[WARNING] test2",
    "[ERROR] test3",
    "[DEBUG] test4",
    "[INFO] streaming test1",
    "[WARNING] streaming test2",
    "[ERROR] streaming test3",
    "[DEBUG] streaming test4",
    ""
  };

  std::string line{};
  std::ifstream file_stream{filename};
  std::size_t i{0};

  while ( ! file_stream.eof() && file_stream.good() )
  {
    getline(file_stream, line);
    CPPUNIT_ASSERT ( line == strings[i] );
    i++;
  }

  if ( file_stream.is_open() )
    file_stream.close();

  int ret = remove(filename.c_str());  // Delete file

  if ( ret == -1 )
  {
    finalcut::FLogger log{};
    log.setOutputStream(std::cerr);
    log.setLineEnding (finalcut::FLog::LF);
    log.error("Cannot delete the test.log file");
  }
}

//----------------------------------------------------------------------
void FLoggerTest::applicationObjectTest()
{
  // Generation of a logger in a shared_ptr via a pointer
  finalcut::FApplication::setLog (std::make_shared<finalcut::FLogger>());
  // Get the shared_ptr with the base class
  std::shared_ptr<finalcut::FLog> log = finalcut::FApplication::getLog();

  std::ostringstream buf{};
  log->setOutputStream(buf);

  log->info("test1");
  CPPUNIT_ASSERT ( buf.str() == "[INFO] test1\r\n" );
  buf.str("");  // Clear buffer

  log->warn("test2");
  CPPUNIT_ASSERT ( buf.str() == "[WARNING] test2\r\n" );
  buf.str("");  // Clear buffer

  log->error("test3");
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] test3\r\n" );
  buf.str("");  // Clear buffer

  log->debug("test4");
  CPPUNIT_ASSERT ( buf.str() == "[DEBUG] test4\r\n" );
  buf.str("");  // Clear buffer

  *log << "test5" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[INFO] test5\r\n" );
  buf.str("");  // Clear buffer

  *log << finalcut::FLog::Error << "test6" << std::flush;
  CPPUNIT_ASSERT ( buf.str() == "[ERROR] test6\r\n" );
  buf.str("");  // Clear buffer

  // Replace the logger with another one
  finalcut::FApplication::setLog(std::make_shared<myLogger>());
  log = finalcut::FApplication::getLog();
  log->setOutputStream(buf);

  log->info("myLogger 1");
  CPPUNIT_ASSERT ( buf.str() == " Info: myLogger 1\n" );
  buf.str("");  // Clear buffer

  log->warn("myLogger 2");
  CPPUNIT_ASSERT ( buf.str() == " Warn: myLogger 2\n" );
  buf.str("");  // Clear buffer

  log->error("myLogger 3");
  CPPUNIT_ASSERT ( buf.str() == "Error: myLogger 3\n" );
  buf.str("");  // Clear buffer

  log->debug("myLogger 4");
  CPPUNIT_ASSERT ( buf.str() == "Debug: myLogger 4\n" );
  buf.str("");  // Clear buffer

  std::shared_ptr<finalcut::FLog>* logger = &(finalcut::FApplication::getLog());
  delete logger;
}


// Put the test suite in the registry
CPPUNIT_TEST_SUITE_REGISTRATION (FLoggerTest);

// The general unit test main part
#include <main-test.inc>

