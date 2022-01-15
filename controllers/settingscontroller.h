#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QSettings>

class SettingsController
{
public:
    SettingsController();

    void saveSettings();
    void loadSettings();
};

#endif // SETTINGSCONTROLLER_H
