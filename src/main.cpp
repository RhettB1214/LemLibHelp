#include "main.h"
#include "lemlib/api.hpp"
#include "definitions.hpp"
#include "UniConLib.h"

ASSET(path2_txt);
ASSET(bar_txt);
ASSET(sillyskills_txt);



/*Variable Definitions*/

	/*ADI Variables*/

		/*Wing Variables*/
		bool wingVal = false;
		bool lastKnowButtonYState;

		/*Hang Variables*/
		bool lastKnowButtonLeftState;
		bool hangUpVal = false;

	/*Motor Variables*/

		/*Cata Variables*/
		bool lastKnowButtonUpState;
		bool lastKnowButtonR1State;
		bool cataAutofire = false;
		bool lastKnowCataSetState = true;
		bool cataSet = true; /*false = up, true = down (ready to fire)*/

/*End of Variable Defintions*/		


/*Device Initializations*/

	/*Drivetrain Initializations*/
	pros::Motor lD1(LD1, SPEEDBOX, true);
	pros::Motor lD2(LD2, SPEEDBOX, true);
	pros::Motor lD3(LD3, SPEEDBOX, false);
	pros::Motor rD1(RD1, SPEEDBOX, false);
	pros::Motor rD2(RD2, SPEEDBOX, false);
	pros::Motor rD3(RD3, SPEEDBOX, true);

	pros::MotorGroup lDrive({lD1, lD2, lD3});
	pros::MotorGroup rDrive({rD1, rD2, rD3});

	pros::Imu imu(IMU_PORT);

	/*Non-DT Initializations*/
	pros::Motor intakeMotor(INTAKE_PORT, REGBOX, false);
	pros::Motor cataMotor(CATA_PORT, TORQUEBOX, false);

	pros::ADIDigitalOut wingPnuem({{EXPAND_PORT, WING_ADIDO}});
	pros::ADIDigitalOut hangPnuem({{EXPAND_PORT, HANG_ADIDO}});
	pros::ADIDigitalIn limitSwitch({{EXPAND_PORT, LIMIT_SWITCH}});
	pros::ADIDigitalOut hangUpPnuem({{EXPAND_PORT, HANG_UP_ADIDO}});

	/*Controller Initialization*/

	pros::Controller master(pros::E_CONTROLLER_MASTER);

/*End of Device Initializations*/


/*LemLib Chassis Initialization*/
lemlib::Drivetrain_t drivetrain 
{
    &lDrive, // left drivetrain motors
    &rDrive, // right drivetrain motors
    11.5, // track width
    4.125, // wheel diameter
    300, // wheel rpm
	4 //chase Power
};

/*LemLib Odometry Sensors Initialization*/
lemlib::OdomSensors_t odomSensors 
{
	nullptr, // left encoder
	nullptr, // right encoder
	nullptr, // back encoder
	nullptr, // front encoder
	&imu // imu
};

/*Lateral (Forwards/Backwards) PID Initialization*/
lemlib::ChassisController_t lateralcontroller 
{
	14, // kP 
    42, // kD  
	1, // smallErrorRange
	100, // smallErrorTimeout
	3, // largeErrorRange
	500, // largeErrorTimeout
	10 // slew rate
};

/*Angular (Turning) PID Initialization*/
lemlib::ChassisController_t angularcontroller
{
	12, // kP
    73, // kD	
	1, // smallErrorRange
	100, // smallErrorTimeout
	3, // largeErrorRange
	500, // largeErrorTimeout
	10 // slew rate
};

/*LemLib Chassis Initialization*/
lemlib::Chassis drive(drivetrain, lateralcontroller, angularcontroller, odomSensors);


/*Function Definitions*/


	/*Brain Screen Functions*/
		/*Write Chassis Pose to the Brain Screen*/
		void screen()
		{
			while (true)
			{
				lemlib::Pose pose = drive.getPose();
				pros::lcd::print(0, "X: %f", pose.x);
				pros::lcd::print(1, "Y: %f", pose.y);
				pros::lcd::print(2, "Theta: %f", pose.theta);
				pros::delay(10);
			}
		}

	/*Chassis Functions*/
	void drve()
	{
		drive.tank(master.get_analog(LeftY), master.get_analog(RightY), 3);
	}

	/*Cata Functions*/
	void load()
	{
  	while (limitSwitch.get_value() == 0)
  	{
    	cataMotor.move_velocity(95);
    	drve();
  	}
  	pros::delay(15);
  	cataMotor.move_velocity(0);   
	}

	void shoot()
	{
  		while (limitSwitch.get_value() == 1)
  		{
    		cataMotor.move_velocity(95);
    		drve();
  		}
  		cataMotor.move_velocity(0);
  		pros::delay(200);
  		load();
	}

	void shootNoAutoReload()
	{
	while (limitSwitch.get_value() == 1)
	{
		cataMotor.move_velocity(95);
		drve();
	}
	cataMotor.move_velocity(0);
	}

	void autoFireCheck()
	{
		while(true)
		{
			if (master.get_digital(Up) != lastKnowButtonUpState)
			{
				lastKnowButtonUpState = master.get_digital(Up);
			
				if (master.get_digital(Up))
				{
					cataAutofire = !cataAutofire;
				}

			}
			pros::delay(10);
		}
	}


void initialize() 
{
	pros::lcd::initialize();
	drive.calibrate();
	pros::Task screenTask(screen);
	wingPnuem.set_value(0);
  	hangPnuem.set_value(0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() 
{

}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() 
{

}

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
void autonomous() 
{
	drive.setPose(-55.297, -56.082, 60);
	lDrive.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	rDrive.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	drive.follow(sillyskills_txt, 5000, 15, false, true);
}
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
void opcontrol() 
{
	lDrive.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	rDrive.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
	pros::Task autoFireCheckTask(autoFireCheck);
	while(true)
  	{
    
    //Drive
    drve();
    cataSet = limitSwitch.get_value();

    	if(master.get_digital(L2))
    	{
      		intakeMotor.move_velocity(200);
    	}
    	else if(master.get_digital(L1))
    	{
      		intakeMotor.move_velocity(-200);
    	}
    	else 
    	{
      		intakeMotor.move_velocity(0);
    	}
    
    

		if (master.get_digital(B) != lastKnowButtonYState)
		{
		lastKnowButtonYState = master.get_digital(B);
		
			if (master.get_digital(B))
			{
				wingVal = !wingVal;
				wingPnuem.set_value(wingVal);
			}
		}

		if (master.get_digital(R2) != lastKnowButtonR1State)
		{
			lastKnowButtonR1State = master.get_digital(R2);
		
		if (master.get_digital(R2) && cataSet == true)
		{
			shoot();
		}
		else if (master.get_digital(R2) && cataSet == false)
		{
			load();
		}
		}

		if (master.get_digital(X) && cataSet == true)
		{
			shootNoAutoReload();
		}
		
		
			
		if (master.get_digital(Left) != lastKnowButtonLeftState)
		{
			lastKnowButtonLeftState = master.get_digital(Left);

		if (master.get_digital(Left) && hangUpVal == false)
		{
			hangUpVal = !hangUpVal;
			hangUpPnuem.set_value(hangUpVal);
			hangPnuem.set_value(0);
			if(cataSet == true)
			{
			shootNoAutoReload();
			}
		}
		else if (master.get_digital(Left) && hangUpVal == true)
		{
			hangUpVal = !hangUpVal;
			hangUpPnuem.set_value(hangUpVal);
			hangPnuem.set_value(1);
			
		
		}
		}
		
		
		
		

		if (cataAutofire) 
		{
		shoot();
		pros::delay(250);
		}
    


  }
}
