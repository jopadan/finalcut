/***********************************************************************
* busy.cpp - Shows the use of the FBusyIndicator                       *
*                                                                      *
* This file is part of the Final Cut widget toolkit                    *
*                                                                      *
* Copyright 2020 Markus Gans                                           *
*                                                                      *
* The Final Cut is free software; you can redistribute it and/or       *
* modify it under the terms of the GNU Lesser General Public License   *
* as published by the Free Software Foundation; either version 3 of    *
* the License, or (at your option) any later version.                  *
*                                                                      *
* The Final Cut is distributed in the hope that it will be useful,     *
* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <final/final.h>
using finalcut::FPoint;
using finalcut::FRect;
using finalcut::FSize;

//----------------------------------------------------------------------
// class Dialog
//----------------------------------------------------------------------

class Dialog final : public finalcut::FDialog
{
  public:
    explicit Dialog (FWidget* parent = nullptr);

  private:
    void adjustSize() override;

    // Event handler
    void onTimer (finalcut::FTimerEvent*) override;

    // Callback method
    void cb_start (const finalcut::FWidget*, const FDataPtr);

    // Data members
    finalcut::FSpinBox seconds{this};
    finalcut::FButton start{"&Start", this};
    finalcut::FButton quit{"&Quit", this};
    finalcut::FBusyIndicator busy_indicator{this};
};

//----------------------------------------------------------------------
Dialog::Dialog (FWidget* parent)
  : finalcut::FDialog{parent}
{
  FDialog::setText ("Dialog");
  finalcut::FDialog::setGeometry (FPoint{26, 5}, FSize{28, 10});
  seconds.setGeometry (FPoint{10, 2}, FSize{10, 1});
  seconds.setLabelText ("Seconds");
  seconds.setRange (0, 60);
  seconds.setValue (3);
  start.setGeometry (FPoint{2, 6}, FSize{10, 1});
  quit.setGeometry (FPoint{15, 6}, FSize{10, 1});

  // Add button callbacks
  seconds.addCallback
  (
    "activate",
    F_METHOD_CALLBACK (this, &Dialog::cb_start)
  );

  start.addCallback
  (
    "clicked",
    F_METHOD_CALLBACK (this, &Dialog::cb_start)
  );

  quit.addCallback
  (
    "clicked",
    F_METHOD_CALLBACK (this, &finalcut::FApplication::cb_exitApp)
  );

}

//----------------------------------------------------------------------
void Dialog::adjustSize()
{
  finalcut::FDialog::adjustSize();
  int X = int((getDesktopWidth() - getWidth()) / 2);
  const int Y = 5;

  if ( X < 1 )
    X = 1;

  setPos (FPoint{X, Y}, false);
}

//----------------------------------------------------------------------
void Dialog::onTimer (finalcut::FTimerEvent*)
{
  delOwnTimer();
  busy_indicator.stop();
}

//----------------------------------------------------------------------
void Dialog::cb_start (const finalcut::FWidget*, const FDataPtr)
{
  if ( seconds.getValue() < 1 )
    return;

  busy_indicator.start();
  addTimer(int(seconds.getValue() * 1000));
}

//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------

int main (int argc, char* argv[])
{
  finalcut::FApplication app(argc, argv);
  Dialog dialog(&app);
  finalcut::FWidget::setMainWidget(&dialog);
  dialog.show();
  return app.exec();
}
