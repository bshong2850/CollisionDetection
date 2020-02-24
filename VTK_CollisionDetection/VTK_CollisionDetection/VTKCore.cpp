#include "VTKCore.h"

VTKCore::VTKCore()
{

}

VTKCore::~VTKCore()
{

}

////Private Function
vtkSmartPointer<vtkPolyData> VTKCore::ReadPolyData(const char *fileName)
{
	vtkSmartPointer<vtkPolyData> polyData;
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(fileName));
	if (extension == ".ply")
	{
		vtkSmartPointer<vtkPLYReader> reader =
			vtkSmartPointer<vtkPLYReader>::New();
		reader->SetFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtp")
	{
		vtkSmartPointer<vtkXMLPolyDataReader> reader =
			vtkSmartPointer<vtkXMLPolyDataReader>::New();
		reader->SetFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".obj")
	{
		vtkSmartPointer<vtkOBJReader> reader =
			vtkSmartPointer<vtkOBJReader>::New();
		reader->SetFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".stl")
	{
		vtkSmartPointer<vtkSTLReader> reader =
			vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".vtk")
	{
		vtkSmartPointer<vtkPolyDataReader> reader =
			vtkSmartPointer<vtkPolyDataReader>::New();
		reader->SetFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else if (extension == ".g")
	{
		vtkSmartPointer<vtkBYUReader> reader =
			vtkSmartPointer<vtkBYUReader>::New();
		reader->SetGeometryFileName(fileName);
		reader->Update();
		polyData = reader->GetOutput();
	}
	else
	{
		vtkSmartPointer<vtkSphereSource> source =
			vtkSmartPointer<vtkSphereSource>::New();
		source->Update();
		polyData = source->GetOutput();
	}
	return polyData;
}

vtkSmartPointer<vtkCollisionDetectionFilter> VTKCore::MeshToMeshCollisionDetect(
	int contactMode,
	vtkSmartPointer<vtkPolyData> polyData1,
	vtkSmartPointer<vtkPolyData> polyData2,
	vtkSmartPointer<vtkMatrix4x4> &polyData1_Matrix,
	vtkSmartPointer<vtkMatrix4x4> &polyData2_Matrix
)
{
	auto collide = vtkSmartPointer<vtkCollisionDetectionFilter>::New();
	collide->SetInputData(0, polyData1);
	collide->SetMatrix(0, polyData1_Matrix);
	collide->SetInputData(1, polyData2);
	collide->SetMatrix(1, polyData2_Matrix);
	collide->SetBoxTolerance(0.0);
	collide->SetCellTolerance(0.0);
	collide->SetNumberOfCellsPerNode(2);
	//collide->RequestData_Test(polyData1, polyData2);
	//collide->PrintSelf();
	KeyPress *KeyFunction = KeyPress::New();
	collide->AddObserver(vtkCommand::KeyPressEvent, KeyFunction);
	return collide;
}

void VTKCore::ComputeCenter(double *bounds, double &center_x, double &center_y, double &center_z)
{
	double xavr = (bounds[1] + bounds[0]) / 2;
	double yavr = (bounds[3] + bounds[2]) / 2;
	double zavr = (bounds[5] + bounds[4]) / 2;
}


////Public Function
void VTKCore::PrintKeyFunction()
{
	std::cout << "KEYBOARD SHORTCUT" << std::endl;
	std::cout << " - Q / E : Exit Program" << std::endl;
	std::cout << " - A / C : Actor Mode     / Camera Mode" << std::endl;
	std::cout << " - W / S : Wireframe Mode / Surface Mode" << std::endl;
	std::cout << " -   R   : Reset Camera View" << std::endl;
	std::cout << std::endl;

	std::cout << " - Mouse Left        : Rotation" << std::endl;
	std::cout << " - Mouse Mid         : Translation" << std::endl;
	std::cout << " - Mouse Right       : Zoom" << std::endl;
	std::cout << " - Mouse Wheel       : Zoom" << std::endl;
	std::cout << " - Ctrl + Mouse Left : Translation" << std::endl;
	std::cout << " - Shift + Mouse Left: XY Rotation" << std::endl;

	std::cout << std::endl;
	std::cout << " -       1  /  2     : Collision Mode Selection" << std::endl;
}

void VTKCore::CollisionDetection()
{
	int windowWidth = 1500;
	int windowHeight = 500;
	int contactMode = 0;
	bool collisionLineSaveMode = false;

	auto polyData_1 = ReadPolyData("model/LowerJawScan_12251.stl");
	auto polyData_2 = ReadPolyData("model/UpperJawScan_16702.stl");
	//auto polyData_2 = ReadPolyData("model/Crown.stl");
	/*auto transform1 = vtkSmartPointer<vtkTransform>::New();
	auto transform2 = vtkSmartPointer<vtkTransform>::New();*/
	auto matrix1 = vtkSmartPointer<vtkMatrix4x4>::New();
	auto matrix2 = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkSmartPointer<vtkCollisionDetectionFilter> collide 
		= MeshToMeshCollisionDetect(contactMode, polyData_1, polyData_2, matrix1, matrix2);

	double bounds[6];
	double xavr = 0;
	double yavr = 0;
	double zavr = 0;
	collide->GetOutput(2)->GetBounds(bounds);
	ComputeCenter(bounds, xavr, yavr, zavr);

	//Collision Line Translattion & Rotation & Scaling
	vtkSmartPointer<vtkTransform> translation_x = vtkSmartPointer<vtkTransform>::New();
	translation_x->Translate(-0.0, 0.0, 0.0);
	translation_x->RotateX(90.0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_x =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_x->SetInputConnection(collide->GetContactsOutputPort());
	transformFilter_x->SetTransform(translation_x);
	transformFilter_x->Update();

	vtkSmartPointer<vtkTransform> translation_y = vtkSmartPointer<vtkTransform>::New();
	translation_y->Translate(-0.0, 0.0, 0.0);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter_y =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter_y->SetInputConnection(collide->GetContactsOutputPort());
	transformFilter_y->SetTransform(translation_y);
	transformFilter_y->Update();

	//Collision Line Position 정보 저장
	if (collisionLineSaveMode)
	{
		vtkSaveVertexCallback *FileSave = vtkSaveVertexCallback::New();
		transformFilter_y->AddObserver(vtkCommand::EndEvent, FileSave);
	}

	// Visualize
	auto colors = vtkSmartPointer<vtkNamedColors>::New();

	//첫번째 Mesh Mapper Actor 정의
	auto polyDataMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	polyDataMapper1->SetInputConnection(collide->GetOutputPort(0));
	polyDataMapper1->ScalarVisibilityOff();

	auto polyDataActor1 = vtkSmartPointer<vtkActor>::New();
	polyDataActor1->SetMapper(polyDataMapper1);
	polyDataActor1->GetProperty()->BackfaceCullingOn();
	polyDataActor1->SetUserMatrix(matrix1);
	polyDataActor1->GetProperty()->SetDiffuseColor(colors->GetColor3d("Tomato").GetData());
	polyDataActor1->GetProperty()->SetRepresentationToWireframe();

	//두번째 Mesh Mapper Actor 정의
	auto polyDataMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	polyDataMapper2->SetInputConnection(collide->GetOutputPort(1));

	auto polyDataActor2 = vtkSmartPointer<vtkActor>::New();
	polyDataActor2->SetMapper(polyDataMapper2);
	polyDataActor2->GetProperty()->BackfaceCullingOn();
	polyDataActor2->SetUserMatrix(matrix2);

	//첫번째 Transform 적용한 Collision Line Mapper Actor 정의
	auto mapper_collision_x = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper_collision_x->SetInputConnection(transformFilter_x->GetOutputPort());
	mapper_collision_x->SetResolveCoincidentTopologyToPolygonOffset();

	auto actor_collision_x = vtkSmartPointer<vtkActor>::New();
	actor_collision_x->SetMapper(mapper_collision_x);
	actor_collision_x->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
	actor_collision_x->GetProperty()->SetLineWidth(3.0);

	//두번째 Transform 적용한 Collision Line Mapper Actor 정의
	auto mapper_collision_y = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper_collision_y->SetInputConnection(transformFilter_y->GetOutputPort());
	mapper_collision_y->SetResolveCoincidentTopologyToPolygonOffset();

	auto actor_collision_y = vtkSmartPointer<vtkActor>::New();
	actor_collision_y->SetMapper(mapper_collision_y);
	actor_collision_y->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
	actor_collision_y->GetProperty()->SetLineWidth(3.0);

	//Frame Rate를 적기 위한 Actor 정의
	auto txt = vtkSmartPointer<vtkTextActor>::New();
	txt->GetTextProperty()->SetFontSize(18);

	//Render 정의
	std::vector<vtkSmartPointer<vtkRenderer> > renderer;
	renderer.push_back(vtkSmartPointer<vtkRenderer>::New());
	renderer.push_back(vtkSmartPointer<vtkRenderer>::New());
	renderer.push_back(vtkSmartPointer<vtkRenderer>::New());
	renderer[0]->SetViewport(0, 0, 1.0 / 3.0, 1);  // Total Viewer
	renderer[1]->SetViewport(1.0 / 3.0, 0, 2.0 / 3.0, 1);  // Top View
	renderer[2]->SetViewport(2.0 / 3.0, 0, 1, 1);  // Front View

	renderer[0]->UseHiddenLineRemovalOn();
	renderer[0]->AddActor(polyDataActor1);
	renderer[0]->AddActor(polyDataActor2);
	renderer[0]->AddActor(txt);
	renderer[0]->SetBackground(colors->GetColor3d("Gray").GetData());

	//Camera View Setting
	vtkSmartPointer<vtkCamera> camera_1 =
		vtkSmartPointer<vtkCamera>::New();
	camera_1->SetPosition(xavr, yavr, 150);
	camera_1->SetFocalPoint(xavr, yavr, -20);
	renderer[1]->UseHiddenLineRemovalOn();
	renderer[1]->AddActor(actor_collision_x);
	renderer[1]->SetActiveCamera(camera_1);
	renderer[1]->SetBackground(colors->GetColor3d("Black").GetData());

	vtkSmartPointer<vtkCamera> camera_2 =
		vtkSmartPointer<vtkCamera>::New();
	camera_2->SetPosition(xavr, yavr, 150);
	camera_2->SetFocalPoint(xavr, yavr, -20);
	renderer[2]->UseHiddenLineRemovalOn();
	renderer[2]->AddActor(actor_collision_y);
	renderer[2]->SetActiveCamera(camera_2);	
	renderer[2]->SetBackground(colors->GetColor3d("Gray").GetData());
	
	//Window에 Renderer 붙여주기
	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer[0]);
	renderWindow->AddRenderer(renderer[1]);
	renderWindow->AddRenderer(renderer[2]);
	renderWindow->SetSize(windowWidth, windowHeight);

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	/*vtkSmartPointer<KeyPressInteractorStyle> style =
		vtkSmartPointer<KeyPressInteractorStyle>::New();*/
	KeyPress *KeyFunction = KeyPress::New();
	//interactor->SetInteractorStyle(style);
	interactor->AddObserver(vtkCommand::KeyPressEvent, KeyFunction);
	//style->SetCurrentRenderer(renderer[1]);
	

	//Frame Rate 계산해서 Viewer에 추가
	vtkFPSCallback *UpdateFPS = vtkFPSCallback::New();
	UpdateFPS->SetTextActor(txt);
	renderer[0]->AddObserver(vtkCommand::EndEvent, UpdateFPS);
	renderWindow->Render();
	renderWindow->SetWindowName("CollisionDetection");
	renderWindow->Render();

	renderer[0]->ResetCamera();
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();

	return ;
}

