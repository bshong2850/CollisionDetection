#pragma once
//#include "VTKCore.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleJoystickActor.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
class KeyPress : public vtkCommand
{
public:
	static KeyPress *New()
	{
		return new KeyPress;
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{

		vtkRenderWindowInteractor *rwi = reinterpret_cast<vtkRenderWindowInteractor *>(caller);
		std::string key = rwi->GetKeySym();

		if (key == "1" || key == "2" || key == "3")
		{
			vtkCollisionDetectionFilter *collide = reinterpret_cast<vtkCollisionDetectionFilter *>(caller);
			if (key == "1")
			{
				std::cout << "Collision Mode : SetCollisionModeToAllContacts Mode" << std::endl;
				collide->SetCollisionModeToAllContacts();
			}
			if (key == "2")
			{
				std::cout << "Collision Mode : SetCollisionModeToFirstContact Mode" << std::endl;
				collide->SetCollisionModeToFirstContact();
			}
			collide->GenerateScalarsOn();
		}
		else
		{
			vtkSmartPointer<vtkInteractorStyle> style;
			if (key == "a")
			{
				std::cout << "Actor Mode     : Trackball Actor Mode" << std::endl;
				style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
				rwi->SetInteractorStyle(style);
				/*vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
				vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();*/
			}
			if (key == "c")
			{
				std::cout << "Actor Mode     : Joystick Actor Mode" << std::endl;
				style = vtkSmartPointer<vtkInteractorStyleJoystickActor>::New();
				rwi->SetInteractorStyle(style);
				/*vtkSmartPointer<vtkInteractorStyleJoystickActor> style =
				vtkSmartPointer<vtkInteractorStyleJoystickActor>::New();*/
			}
			if (key == "t")
			{
				std::cout << "Camera Mode    : Trackball Camera Mode" << std::endl;
				style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
				rwi->SetInteractorStyle(style);
				/*vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
				vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();*/

			}
			if (key == "j")
			{
				std::cout << "Camera Mode    : Joystick Camera Mode" << std::endl;
				style = vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();
				rwi->SetInteractorStyle(style);
				/*vtkSmartPointer<vtkInteractorStyleJoystickCamera> style =
				vtkSmartPointer<vtkInteractorStyleJoystickCamera>::New();*/

			}
			if (key == "s")
			{
				std::cout << "Rendering Mode : Surface Mode" << std::endl;
			}
			if (key == "w")
			{
				std::cout << "Rendering Mode : Wireframe Mode" << std::endl;
			}
		}

	}

};