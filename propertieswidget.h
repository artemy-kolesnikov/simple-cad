#ifndef PROPERTIES_WIDGET_HEADER
#define PROPERTIES_WIDGET_HEADER

#include <QWidget>

#include <Graphic3d_NameOfMaterial.hxx>

class Model;
class QComboBox;
class QCheckBox;

/**
 * Model properties widget
 */
class PropertiesWidget : public QWidget
{
	Q_OBJECT
public:
	PropertiesWidget(QWidget* parent = 0);

	void setModel(Model* model);

Q_SIGNALS:
	void materialChanged(Graphic3d_NameOfMaterial material);
	void shadedChanged(bool shaded);

private Q_SLOTS:
	void materialChanged(int);
	void shaddedChanged(int);

private:
	void createUI();
	void updateView();

private:
	Model* model;

	QComboBox* cbMaterial;
	QCheckBox* cbShaded;
};

#endif // PROPERTIES_WIDGET_HEADER

