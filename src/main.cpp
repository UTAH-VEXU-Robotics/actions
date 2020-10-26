#include "main.h"

/**
 * You should add more #includes here
 */
#include "okapi/api.hpp" // <-- UNCOMMENT THIS LINE
#include <iostream>

using namespace okapi;
using namespace okapi::literals;


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  Controller controller;
  ControllerButton fullUpIntake(ControllerDigital::R1);
  ControllerButton bottomRollerUp(ControllerDigital::R2);
  ControllerButton topRollerUp(ControllerDigital::L1);
  ControllerButton fullDownIntake(ControllerDigital::L2);


  MotorGroup test ({1,2,3,4,5,6,7,8,9,10,11,12,13,14,-15,16,17,18,19,20});

  //drive
  MotorGroup rightDrive({-7,-1});
  MotorGroup leftDrive({-8,-6});

  std::shared_ptr<ChassisController> drive =
      ChassisControllerBuilder()
          .withMotors(leftDrive, rightDrive)
              // Green gearset, 4 in wheel diam, 11.5 in wheel track
          .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
          .build();

  //intake
  MotorGroup bottomIntake({-12,16});
  Motor bottomRoller(15);
  Motor topRoller(17);

	while (true) {

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

    drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftX),
                              controller.getAnalog(ControllerAnalog::rightY));

    if(fullUpIntake.isPressed()){
      bottomRoller.moveVoltage(12000);
      topRoller.moveVoltage(12000);
      bottomIntake.moveVoltage(12000);
    }else if(bottomRollerUp.isPressed()){
      bottomRoller.moveVoltage(12000);
    }else if(topRollerUp.isPressed()){
      topRoller.moveVoltage(12000);
    }else if(fullDownIntake.isPressed()){
      bottomRoller.moveVoltage(-12000);
      topRoller.moveVoltage(-12000);
      bottomIntake.moveVoltage(-12000);
    }else{
      bottomRoller.moveVoltage(0);
      topRoller.moveVoltage(0);
      bottomIntake.moveVoltage(0);
    }



    pros::delay(10);


	}
}
