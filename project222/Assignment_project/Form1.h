#pragma once

// ⭐ 1. ส่วนสำคัญ: ต้องอยู่บนสุดเพื่อเรียกใช้คำสั่งเมาส์ของ Windows ⭐
#include <Windows.h>
#pragma comment(lib, "user32.lib") 

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

// ⭐ 2. แก้ชื่อไฟล์ให้ตรงกับไฟล์จริงของคุณ (MyForm333.h) ⭐
#include "MyForm333.h"

// Namespaces
using namespace std;
using namespace cv;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace Assignment_project {

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			// เริ่มต้นระบบ
			capture = new cv::VideoCapture();
			frame = new cv::Mat();
			faceDetector = new cv::CascadeClassifier();

			// โหลดโมเดลจับหน้า
			if (!faceDetector->load("haarcascade_frontalface_alt2.xml")) {
				MessageBox::Show("หาไฟล์ haarcascade_frontalface_alt2.xml ไม่เจอ! กรุณาเช็คว่าวางไฟล์ไว้ในโฟลเดอร์เดียวกับ .exe หรือยัง");
			}

			// โหลดโมเดลจับตา
			eyeDetector = new cv::CascadeClassifier();
			if (!eyeDetector->load("haarcascade_eye.xml")) {
				MessageBox::Show("หาไฟล์ haarcascade_eye.xml ไม่เจอ! โหลดหรือยังครับ?");
			}

			isCameraRunning = false;
		}

	protected:
		~Form1()
		{
			if (timer1 != nullptr) timer1->Stop();
			if (components) delete components;

			if (capture != nullptr) {
				if (capture->isOpened()) capture->release();
				delete capture;
				capture = nullptr;
			}

			if (frame != nullptr) { delete frame; frame = nullptr; }
			if (faceDetector != nullptr) { delete faceDetector; faceDetector = nullptr; }
			if (eyeDetector != nullptr) { delete eyeDetector; eyeDetector = nullptr; }
		}

		// --- ประกาศตัวแปร Global ---
	private:
		cv::VideoCapture* capture = nullptr;
		cv::Mat* frame = nullptr;
		cv::CascadeClassifier* faceDetector = nullptr;
		cv::CascadeClassifier* eyeDetector = nullptr;

		int blinkCounter = 0;
		bool isClicking = false;
		bool isCameraRunning = false;

		// ตัวแปรสำหรับ Mouse Control
		int faceX = 0;
		int faceY = 0;

		// ⭐ ตัวแปรเก็บค่า Calibration (หน้าตรง/ซ้าย/ขวา) ⭐
		int calibCenter = 0;
		int calibCenterY = 0; // เก็บค่า Y ตอนหน้าตรง
		int calibLeft = 0;
		int calibRight = 0;

		// --- Controls จากหน้า Design ---
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ btnStart;
	private: System::Windows::Forms::Button^ btnCapCenter;
	private: System::Windows::Forms::Button^ btnCapLeft;
	private: System::Windows::Forms::Button^ btnCapRight;
	private: System::Windows::Forms::Label^ lblStatus;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Button^ btnSave;
	private: System::Windows::Forms::Label^ lblAction;
	private: System::Windows::Forms::Button^ btnNurseCall;
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			   this->btnStart = (gcnew System::Windows::Forms::Button());
			   this->btnCapCenter = (gcnew System::Windows::Forms::Button());
			   this->btnCapLeft = (gcnew System::Windows::Forms::Button());
			   this->btnCapRight = (gcnew System::Windows::Forms::Button());
			   this->lblStatus = (gcnew System::Windows::Forms::Label());
			   this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			   this->btnSave = (gcnew System::Windows::Forms::Button());
			   this->lblAction = (gcnew System::Windows::Forms::Label());
			   this->btnNurseCall = (gcnew System::Windows::Forms::Button());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // pictureBox1
			   // 
			   this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			   this->pictureBox1->Location = System::Drawing::Point(16, 15);
			   this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			   this->pictureBox1->Name = L"pictureBox1";
			   this->pictureBox1->Size = System::Drawing::Size(853, 590);
			   this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			   this->pictureBox1->TabIndex = 0;
			   this->pictureBox1->TabStop = false;
			   // 
			   // btnStart
			   // 
			   this->btnStart->BackColor = System::Drawing::Color::Lime;
			   this->btnStart->Location = System::Drawing::Point(16, 628);
			   this->btnStart->Margin = System::Windows::Forms::Padding(4);
			   this->btnStart->Name = L"btnStart";
			   this->btnStart->Size = System::Drawing::Size(133, 49);
			   this->btnStart->TabIndex = 1;
			   this->btnStart->Text = L"เปิดกล้อง";
			   this->btnStart->UseVisualStyleBackColor = false;
			   this->btnStart->Click += gcnew System::EventHandler(this, &Form1::btnStart_Click);
			   // 
			   // btnCapCenter
			   // 
			   this->btnCapCenter->Location = System::Drawing::Point(160, 628);
			   this->btnCapCenter->Margin = System::Windows::Forms::Padding(4);
			   this->btnCapCenter->Name = L"btnCapCenter";
			   this->btnCapCenter->Size = System::Drawing::Size(160, 49);
			   this->btnCapCenter->TabIndex = 2;
			   this->btnCapCenter->Text = L"จับภาพหน้าตรง";
			   this->btnCapCenter->UseVisualStyleBackColor = true;
			   this->btnCapCenter->Click += gcnew System::EventHandler(this, &Form1::btnCapCenter_Click);
			   // 
			   // btnCapLeft
			   // 
			   this->btnCapLeft->Location = System::Drawing::Point(333, 628);
			   this->btnCapLeft->Margin = System::Windows::Forms::Padding(4);
			   this->btnCapLeft->Name = L"btnCapLeft";
			   this->btnCapLeft->Size = System::Drawing::Size(160, 49);
			   this->btnCapLeft->TabIndex = 3;
			   this->btnCapLeft->Text = L"จับภาพหันซ้าย";
			   this->btnCapLeft->UseVisualStyleBackColor = true;
			   this->btnCapLeft->Click += gcnew System::EventHandler(this, &Form1::btnCapLeft_Click);
			   // 
			   // btnCapRight
			   // 
			   this->btnCapRight->Location = System::Drawing::Point(507, 628);
			   this->btnCapRight->Margin = System::Windows::Forms::Padding(4);
			   this->btnCapRight->Name = L"btnCapRight";
			   this->btnCapRight->Size = System::Drawing::Size(160, 49);
			   this->btnCapRight->TabIndex = 4;
			   this->btnCapRight->Text = L"จับภาพหันขวา";
			   this->btnCapRight->UseVisualStyleBackColor = true;
			   this->btnCapRight->Click += gcnew System::EventHandler(this, &Form1::btnCapRight_Click);
			   // 
			   // lblStatus
			   // 
			   this->lblStatus->AutoSize = true;
			   this->lblStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->lblStatus->Location = System::Drawing::Point(887, 37);
			   this->lblStatus->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			   this->lblStatus->Name = L"lblStatus";
			   this->lblStatus->Size = System::Drawing::Size(167, 25);
			   this->lblStatus->TabIndex = 5;
			   this->lblStatus->Text = L"สถานะ: รอเปิดกล้อง";
			   // 
			   // timer1
			   // 
			   this->timer1->Interval = 30;
			   this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			   // 
			   // btnSave
			   // 
			   this->btnSave->Location = System::Drawing::Point(683, 628);
			   this->btnSave->Name = L"btnSave";
			   this->btnSave->Size = System::Drawing::Size(160, 49);
			   this->btnSave->TabIndex = 6;
			   this->btnSave->Text = L"บันทึกข้อมูล";
			   this->btnSave->UseVisualStyleBackColor = true;
			   this->btnSave->Click += gcnew System::EventHandler(this, &Form1::btnSave_Click);
			   // 
			   // lblAction
			   // 
			   this->lblAction->AutoSize = true;
			   this->lblAction->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			   this->lblAction->ForeColor = System::Drawing::Color::Red;
			   this->lblAction->Location = System::Drawing::Point(887, 93);
			   this->lblAction->Name = L"lblAction";
			   this->lblAction->Size = System::Drawing::Size(73, 25);
			   this->lblAction->TabIndex = 7;
			   this->lblAction->Text = L"Action:";
			   // 
			   // btnNurseCall
			   // 
			   this->btnNurseCall->BackColor = System::Drawing::SystemColors::ActiveCaption;
			   this->btnNurseCall->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->btnNurseCall->Location = System::Drawing::Point(892, 149);
			   this->btnNurseCall->Name = L"btnNurseCall";
			   this->btnNurseCall->Size = System::Drawing::Size(162, 163);
			   this->btnNurseCall->TabIndex = 8;
			   this->btnNurseCall->Text = L"Nurse Call !!!";
			   this->btnNurseCall->UseVisualStyleBackColor = false;
			   this->btnNurseCall->Click += gcnew System::EventHandler(this, &Form1::btnNurseCall_Click);
			   // 
			   // Form1
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1067, 738);
			   this->Controls->Add(this->btnNurseCall);
			   this->Controls->Add(this->lblAction);
			   this->Controls->Add(this->btnSave);
			   this->Controls->Add(this->lblStatus);
			   this->Controls->Add(this->btnCapRight);
			   this->Controls->Add(this->btnCapLeft);
			   this->Controls->Add(this->btnCapCenter);
			   this->Controls->Add(this->btnStart);
			   this->Controls->Add(this->pictureBox1);
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"Form1";
			   this->Text = L"NeuroCursor - Head Tracking";
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

		   // --- ฟังก์ชันแปลงภาพ Mat เป็น Bitmap ---
		   System::Drawing::Bitmap^ MatToBitmap(cv::Mat& src) {
			   if (src.empty()) return nullptr;
			   cv::Mat temp;
			   if (src.channels() == 1) {
				   cv::cvtColor(src, temp, cv::COLOR_GRAY2BGR);
			   }
			   else {
				   src.copyTo(temp);
			   }
			   System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap(
				   temp.cols, temp.rows,
				   System::Drawing::Imaging::PixelFormat::Format24bppRgb
			   );
			   System::Drawing::Imaging::BitmapData^ bmpData = bitmap->LockBits(
				   System::Drawing::Rectangle(0, 0, temp.cols, temp.rows),
				   System::Drawing::Imaging::ImageLockMode::WriteOnly,
				   bitmap->PixelFormat
			   );
			   size_t dataSize = temp.step * temp.rows;
			   memcpy(bmpData->Scan0.ToPointer(), temp.data, dataSize);
			   bitmap->UnlockBits(bmpData);
			   return bitmap;
		   }

		   // --- Event Handlers ---
	private: System::Void btnStart_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!isCameraRunning) {
			capture->open(0);
			if (capture->isOpened()) {
				timer1->Start();
				btnStart->Text = L"ปิดกล้อง";
				isCameraRunning = true;
			}
			else {
				MessageBox::Show(L"เปิดกล้องไม่ได้!");
			}
		}
		else {
			timer1->Stop();
			capture->release();
			btnStart->Text = L"เปิดกล้อง";
			isCameraRunning = false;
			pictureBox1->Image = nullptr;
		}
	}

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		if (capture->isOpened()) {
			*capture >> *frame;
			if (!frame->empty()) {
				// กลับด้านภาพ (Mirror) เพื่อให้ควบคุมเป็นธรรมชาติ
				cv::flip(*frame, *frame, 1);
				cv::Mat gray;
				cv::cvtColor(*frame, gray, cv::COLOR_BGR2GRAY);

				std::vector<cv::Rect> faces;
				faceDetector->detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(100, 100));

				if (faces.size() > 0) {
					cv::rectangle(*frame, faces[0], cv::Scalar(0, 255, 0), 2);

					faceX = faces[0].x + faces[0].width / 2;
					faceY = faces[0].y + faces[0].height / 2;

					// =========================================================
					// ⭐ 1. ส่วนควบคุมเมาส์ (Head Tracking Mouse) ⭐
					// =========================================================
					if (calibCenter != 0 && calibCenterY != 0) {
						// ปรับความไวเมาส์ตรงนี้ (Sensitivity) 
						// ยิ่งเลขมาก เมาส์ยิ่งพุ่งเร็ว
						int sensitivityX = 12;
						int sensitivityY = 10;

						int screenW = GetSystemMetrics(SM_CXSCREEN);
						int screenH = GetSystemMetrics(SM_CYSCREEN);

						int diffX = faceX - calibCenter;
						int diffY = faceY - calibCenterY;

						// คำนวณพิกัดเป้าหมาย
						int targetX = (screenW / 2) + (diffX * sensitivityX);
						int targetY = (screenH / 2) + (diffY * sensitivityY);

						// สั่งย้ายเมาส์จริง
						SetCursorPos(targetX, targetY);
					}

					// =========================================================
					// ⭐ 2. ตรวจจับการกะพริบตา (Blink Detection) ⭐
					// =========================================================
					cv::Rect eyeArea(
						faces[0].x,
						faces[0].y + (faces[0].height * 0.2),
						faces[0].width,
						faces[0].height * 0.4
					);
					eyeArea = eyeArea & cv::Rect(0, 0, frame->cols, frame->rows);

					cv::Mat faceROI = gray(eyeArea);
					std::vector<cv::Rect> eyes;

					if (eyeDetector != nullptr) {
						eyeDetector->detectMultiScale(faceROI, eyes, 1.1, 2, 0, cv::Size(20, 20));
					}

					for (int i = 0; i < eyes.size(); i++) {
						cv::Point eyeCenter(
							faces[0].x + eyes[i].x + eyes[i].width / 2,
							faces[0].y + (faces[0].height * 0.2) + eyes[i].y + eyes[i].height / 2
						);
						int radius = cvRound((eyes[i].width + eyes[i].height) * 0.25);
						cv::circle(*frame, eyeCenter, radius, cv::Scalar(255, 0, 0), 2);
					}

					// ---------------------------------------------------------
					// ⭐ Logic: คลิกซ้าย/ขวา ⭐
					// ---------------------------------------------------------
					if (eyes.size() == 0) {
						// --- กรณีหลับตา ---
						blinkCounter++;
						if (blinkCounter < 6) {
							lblAction->Text = L"Action: ...";
							lblAction->ForeColor = System::Drawing::Color::Gray;
						}
						else if (blinkCounter >= 6 && blinkCounter < 30) {
							lblAction->Text = L"Action: Charging [Left Click]... " + blinkCounter;
							lblAction->ForeColor = System::Drawing::Color::Orange;
						}
						else if (blinkCounter >= 30 && blinkCounter < 60) {
							lblAction->Text = L"Action: Charging [Right Click]... " + blinkCounter;
							lblAction->ForeColor = System::Drawing::Color::Blue;
						}
						else {
							lblAction->Text = L"Action: Time Out";
							lblAction->ForeColor = System::Drawing::Color::Red;
						}
					}
					else {
						// --- กรณีลืมตา (สั่งคลิก) ---
						if (!isClicking) {
							// 🟢 คลิกซ้าย
							if (blinkCounter >= 6 && blinkCounter < 30) {
								lblAction->Text = L">> LEFT CLICK! <<";
								lblAction->ForeColor = System::Drawing::Color::Green;

								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
								isClicking = true;
							}
							// 🔵 คลิกขวา
							else if (blinkCounter >= 30 && blinkCounter < 60) {
								lblAction->Text = L">> RIGHT CLICK! <<";
								lblAction->ForeColor = System::Drawing::Color::Blue;

								mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
								isClicking = true;
							}
							else if (blinkCounter > 0) {
								lblAction->Text = L"Action: Ready";
								lblAction->ForeColor = System::Drawing::Color::Black;
								isClicking = false;
							}
						}
						else {
							lblAction->Text = L"Action: Ready";
							lblAction->ForeColor = System::Drawing::Color::Black;
							isClicking = false;
						}
						blinkCounter = 0;
					}
					lblStatus->Text = L"สถานะ: เจอหน้า X:" + faceX + L" Y:" + faceY;
				}
				else {
					lblStatus->Text = L"สถานะ: มองไม่เห็นหน้า";
					lblAction->Text = L"Action: ...";
					lblAction->ForeColor = System::Drawing::Color::Black;
					blinkCounter = 0;
				}
				pictureBox1->Image = MatToBitmap(*frame);
			}
		}
	}

		   // 1. ปุ่มจับภาพหน้าตรง
	private: System::Void btnCapCenter_Click(System::Object^ sender, System::EventArgs^ e) {
		if (isCameraRunning && faceX != 0) {
			calibCenter = faceX;
			calibCenterY = faceY; // ⭐ จำ Y ด้วย
			lblStatus->Text = L"บันทึกหน้าตรงแล้ว! (X=" + calibCenter + ", Y=" + calibCenterY + ")";
		}
		else {
			MessageBox::Show(L"ยังไม่เปิดกล้อง หรือ ไม่เจอใบหน้า!");
		}
	}

		   // 2. ปุ่มจับภาพหันซ้าย
	private: System::Void btnCapLeft_Click(System::Object^ sender, System::EventArgs^ e) {
		if (isCameraRunning && faceX != 0) {
			calibLeft = faceX;
			lblStatus->Text = L"บันทึกหันซ้ายแล้ว! (X=" + calibLeft + ")";
		}
	}

		   // 3. ปุ่มจับภาพหันขวา
	private: System::Void btnCapRight_Click(System::Object^ sender, System::EventArgs^ e) {
		if (isCameraRunning && faceX != 0) {
			calibRight = faceX;
			lblStatus->Text = L"บันทึกหันขวาแล้ว! (X=" + calibRight + ")";
		}
	}

	private: System::Void btnSave_Click(System::Object^ sender, System::EventArgs^ e) {
		if (calibCenter == 0 || calibLeft == 0 || calibRight == 0) {
			MessageBox::Show(L"กรุณาจับภาพให้ครบ 3 ท่าก่อนครับ!");
			return;
		}
		try {
			System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter("calibration.txt");
			sw->WriteLine(calibCenter);
			sw->WriteLine(calibLeft);
			sw->WriteLine(calibRight);
			sw->WriteLine(calibCenterY); // เพิ่มการบันทึก Y ลงไฟล์
			sw->Close();
			MessageBox::Show(L"บันทึกไฟล์ calibration.txt เรียบร้อย!");
		}
		catch (System::Exception^ ex) {
			MessageBox::Show(L"บันทึกไม่สำเร็จ: " + ex->Message);
		}
	}

		   // ============================================
		   // ⭐ 4. ฟังก์ชันกดปุ่ม Nurse Call เพื่อเปิดหน้าใหม่ ⭐
		   // ============================================
	private: System::Void btnNurseCall_Click(System::Object^ sender, System::EventArgs^ e) {
		// เรียกใช้ MyForm333 ตามชื่อไฟล์จริงของคุณ
		MyForm333^ f3 = gcnew MyForm333();
		f3->ShowDialog();
	}

	};
}