#include "settingscontroller.h"

SettingsController::SettingsController()
{

}

void SettingsController::saveSettings()
{
    QSettings settings("BMC", "MediaPlayer");
    settings.beginGroup("main");
    settings.setValue("volume", 10);
    settings.endGroup();

}

void SettingsController::loadSettings()
{
    QSettings settings("BMC", "MediaPlayer");
    settings.beginGroup("main");
    int val = settings.value("volume").toInt();
    settings.endGroup();
}
