#ifndef PLANE_SETTINGS_WIDGET_HEADER
#define PLANE_SETTINGS_WIDGET_HEADER

#include <QWidget>

class PlaneSettingsWidget : public QWidget
{
	Q_OBJECT
public:
	PlaneSettingsWidget(QWidget* parent = 0);

private:
	void createUI();
};

#endif // PLANE_SETTINGS_WIDGET_HEADER

