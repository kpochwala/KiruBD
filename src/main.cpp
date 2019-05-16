#include <Arduino.h>
#include "Keyboard.h"

/***********************
 *      Utilities      *
 ***********************/

/**
 * For the keyboard functions to work they need a small
 * delay between them. If you notice there's some bugs
 * when running your arduino, try using different delay values
 * on this function.
 */
void rdDelay() {
  delay(150);
}

/**
 * A longer delay (5 times the regular delay).
 */
void rdLongerDelay() {
  for(int i = 0; i < 5; i++) rdDelay();
}

/**
 * Same as (Received hold key) + (target key).
 */
void rdKeyCombination(uint8_t holdKey, uint8_t targetKey) {
  Keyboard.press(holdKey);
  rdDelay();
  Keyboard.press(targetKey);
  rdDelay();
  Keyboard.releaseAll();
  rdDelay();
}

/**
 * Same as (Received hold key 1) + (received hold key 2) + (target key).
 */
void rdKeyCombination(uint8_t holdKey1, uint8_t holdKey2, uint8_t targetKey) {
  Keyboard.press(holdKey1);
  rdDelay();
  rdKeyCombination(holdKey2, targetKey);
}
/**
 * Same as above but with one more hold key.
 */
void rdKeyCombination(uint8_t holdKey1, uint8_t holdKey2, uint8_t holdKey3, uint8_t targetKey) {
  Keyboard.press(holdKey1);
  rdDelay();
  rdKeyCombination(holdKey2, holdKey3, targetKey);
}

/**
 * Same as Gui + (the received key)
 */
void rdGuiCombination(uint8_t c) {
  rdKeyCombination(KEY_LEFT_GUI, c);
}

/**
 * Same as Alt + (the received key)
 */
void rdAltCombination(uint8_t c) {
  rdKeyCombination(KEY_LEFT_ALT, c);
}

/**
 * Same as Ctrl + (the received key)
 */
void rdCtrlCombination(uint8_t c) {
  rdKeyCombination(KEY_LEFT_CTRL, c);
}

/**
 * Prepares the virtual keyboard.
 * This method should be the first instruction on the setup code block.
 */
void start() {
  Keyboard.begin();
  rdLongerDelay();
}

/**
 * Ends the virtual keyboard.
 * This method should be the last instruction on the setup code block.
 */
void finish() {
  Keyboard.end();
}



void rdWriteText(String text) {
  Keyboard.print(text);
  rdDelay();
}

/**
 * Used to type non-alphanumeric keys.
 */
void rdTypeKey(uint8_t key)
{
  Keyboard.press(key);
  rdDelay();
  Keyboard.release(key);
  rdDelay();
}


/**
 * Accepts the windows smart screen to grant admin permissions.
 */
void rdAcceptWindowsSmartScreen() {
  // Wait untill smart screen shows up
  rdLongerDelay();
  rdTypeKey(KEY_LEFT_ARROW);
  rdDelay();
  Keyboard.print(F(" "));
  rdDelay();
}

/***********************
 *      Libraries      *
 ***********************
 * You may remove the  *
 * unused functions    *
 * before uploading    *
 * the code to the     *
 * arduino             *
 ***********************/


/**
 * Runs a program.
 * Example: "notepad" starts notepad, "calc" starts the calculator.
 */
void rdRun(String program) {
  rdGuiCombination('r');
  Keyboard.print(program);
  rdDelay();
  rdTypeKey(KEY_RETURN);
}

/**
 * Takes a screenshot.
 */
void rdPrintScreen() {
  // some machines use 206 key as the PrtScreen key
  // others might use 229, and others might use both so
  // we use both instructions
  rdTypeKey(206);
  rdTypeKey(229);
  Keyboard.print(F("h"));
  rdDelay();
  Keyboard.print(F("b"));
  rdDelay();
}

/**
 * Opens the JavaScript console on a browser.
 */
void rdOpenJavascriptConsole() {
  rdKeyCombination(KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'i');
}

/**
 * Hides a window:
 * Basically it drags a window to the lowest it can be
 * and then repositions the cursor.
 */
void rdHideWindow() {
  rdAltCombination(' ');
  Keyboard.print(F("M"));
  rdDelay();
  Keyboard.press(KEY_DOWN_ARROW);
  // 100 should be enough to guarantee the window is as low as possible
  // also please notice that 100 is not the real number of strokes since
  // some of the strokes are ignored.
  for(int i = 0; i < 10; i++) rdLongerDelay();;
  Keyboard.release(KEY_DOWN_ARROW);
  // return repositions the cursor back to its original position
  rdTypeKey(KEY_RETURN);
}

/**
 * Same as Win + D
 */
void rdShowDesktop() {
  rdGuiCombination('d');
}

/**
 * Same as Ctrl + V
 */
void rdPaste() {
  rdCtrlCombination('v');
}

/**
 * Same as Ctrl + X
 */
void rdCut() {
  rdCtrlCombination('x');
}

/**
 * Same as Ctrl + C
 */
void rdCopy() {
  rdCtrlCombination('c');
}





/**
 * Same as Shift + (the received key).
 */
void rdShiftCombination(uint8_t c) {
  rdKeyCombination(KEY_LEFT_SHIFT, c);
}





/**
 * Opens the command prompt, if the "admin" parameter
 * has a "true value", it opens a command prompt
 * with admin rights. Or without admin rights otherwise.
 */
void rdOpenCommandPrompt(boolean admin) {
  if (admin) {
    rdGuiCombination('x');
    Keyboard.print(F("a"));
    rdAcceptWindowsSmartScreen();
  } else {
    rdRun("cmd");
  }
}

/**
 * Opens the command prompt without admin rights.
 */
void rdOpenCommandPrompt() {
  rdOpenCommandPrompt(false);
}



/**
 * Changes the keyboard layout, if the computer only
 * has 1 keyboard layout this key combination won't
 * do anything.
 */
void rdChangeKeyboardLayout() {
  rdAltCombination(KEY_LEFT_SHIFT);
}

/**
 * It runs one or multiple powershell scripts,
 * to run multiple scripts, separate them with a new line "\n" char.
 */
void rdPowershellRun(String scripts) {
  char delimiter = '\n';
  String finalScript = "powershell ";
  while (scripts.indexOf('\n') > 0) {
    finalScript = finalScript + "(" + scripts.substring(0, scripts.indexOf('\n')) + ") ; ";
    scripts = scripts.substring(scripts.indexOf('\n') + 1);
  }
  finalScript = finalScript + "(" + scripts + ")";
  rdRun(finalScript);
}

/*********************
 *      Arduino      *
 *********************/

void changeDesktop()
{
  rdChangeKeyboardLayout(); // Turns the keyboard layout to american
  rdPowershellRun(
    "Start-BitsTransfer -Source 'http://goo.gl/vzqBFS' -Destination ($env:USERPROFILE + '\\Desktop\\CAT.jpg')\n" // Downloads the image
    "mspaint ($env:USERPROFILE + '\\Desktop\\CAT.jpg')" // Opens the downloaded image on paint
  );
  for (int i = 0; i < 6; i++) rdLongerDelay(); // Might need to be changed, depending on the machine's internet speed
  rdTypeKey(KEY_LEFT_ALT); // Activates alt shortcuts
  rdWriteText("f"); // File
  rdWriteText("b"); // Set as desktop background
  rdAltCombination(KEY_F4); // Closes paint
  rdPowershellRun(
    "Remove-Item ($env:USERPROFILE + '\\Desktop\\CAT.jpg')" // Deletes the image
  );
rdChangeKeyboardLayout(); // Turns the keyboard layout back to normal
}

void setup() {
  start();

  changeDesktop();

  rdRun("notepad");

  rdAltCombination(' ');
  rdTypeKey('x');
  rdAltCombination('o');
  rdTypeKey('f');
  rdTypeKey(KEY_TAB);
  rdTypeKey(KEY_TAB);
  rdWriteText("20");
  rdTypeKey(KEY_RETURN);

  rdWriteText("STO LAT \n");
  delay(500);
  rdWriteText("STO LAT \n");
  delay(500);
  rdWriteText("NIECH ZYJE ZYJE NAM \n");
  delay(500);
  rdWriteText("A KTO? \n");
  delay(500);
  rdWriteText("KIRU! \n\n\n");
  delay(500);
  
  rdWriteText("Zebys nigdy z parametrycznego drzewa nie spadl, \nstudia Ci lekkimi byly i obronil w końcu Inz. ;) \n\n");
  delay(3000);
  rdWriteText("Zarowno na architekturze, jak i na infie - to drugie sensowniejsze :P \n");
  delay(3000);
  rdWriteText("                                           (THIS FILLS YOU WITH DETERMINATION)\n\n");
  delay(3000);

  rdWriteText("W miedzyczasie wez to Arduino Leonardo \n");
  delay(1000);
  rdWriteText("(jak ten wynalazca Da Vinci - on w sumie i malowal i wynalazki robil) \n");
  delay(2000);
  rdWriteText("I baw sie - jak masz ochote zrodla tej kartki znajdziesz ponizej \n\n");
  delay(3000);
  rdWriteText("Wystarczy sciagnac, odpalic platformio/arduino and have fun!:\n\n https://github.com/kpochwala/KiruBD\n\n");


  delay(3000);
  rdWriteText("Lalalalala\n\n");

  delay(3000);
  rdWriteText("tyryryry\n\n");

  delay(3000);
  rdWriteText("nie zwracaj na mnie uwagi\n\n");

  delay(3000);
  rdWriteText("wcale nie robie niczego w tle\n\n");

  delay(3000);
  rdWriteText("ABSOLUTNIE nie\n\n");

  delay(3000);
  rdWriteText("OK juz.\n\n");
  
  delay(3000);
  rdWriteText("Autodestrukcja za 5...\n\n");
  delay(1000);
  rdWriteText("4...\n\n");
  delay(1000);
  rdWriteText("3...\n\n");
  delay(1000);
  rdWriteText("2...\n\n");
  delay(1000);
  rdWriteText("1...\n\n");
  delay(1000);

  rdWriteText("0...\n\n");
  delay(1000);
  rdWriteText("0...\n\n");
  delay(1000);
  rdWriteText("0...\n\n");
  delay(1000);

  rdWriteText("0...\n\n");
  delay(2000);
  rdWriteText("-1?\n\n");
  delay(2000);

  rdWriteText("???\n\n");
  delay(3000);
  rdWriteText("Dobra, nevermind.\n\n");
  delay(2000);
  rdWriteText("W kazdym razie - jeszcze raz milej zabawy i najlepszego!!!\n\n");
  delay(1000);

finish();
}

void loop() {}