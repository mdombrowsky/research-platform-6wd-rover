//////////////////////////////////////////////////////////////////////////
//file name:
//project name:
//designer:     Michael Dombrowsky
//created on:   06/02/2025
//last modification:  06/07/2020
//version:      0.0.1
//description:    test project to learn
//notes:
//ide:        AVR Studio 6.1.2730 SP2
//compiler      AVR Toolchain 4.8.4.371, gcc 4.8.4
//cpu config:
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// still to do
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Include Header
//////////////////////////////////////////////////////////////////////////

// Basic Header

// Atmel, AVR

// Arduino
// remove all headers in here when arduino routines are replaced with avr
#include <Wire.h>

// FreeRTOS
//#include <FreeRTOS.h>
//#include <task.h>
//#include <timers.h>
//#include <semphr.h>

// MicroROS
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>

#include <std_msgs/msg/int32.h>
#include <sensor_msgs/msg/imu.h>
//#include <std_msgs/msg/int32_multi_array.h>


//////////////////////////////////////////////////////////////////////////
// Macros and Definitions
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// hardware config
//////////////////////////////////////////////////////////////////////////

// Arduino DUE, cortex-m3
// IMU, I2C-Address for MPU6050: 0x68

//////////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Structures and DataTypes
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Function Prototypes and Forward Declarations
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Global Scope Objects and Static Member Definitions
//////////////////////////////////////////////////////////////////////////





#define LED_PIN 13
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if ((temp_rc != RCL_RET_OK)) return false; }
#define EXECUTE_EVERY_N_MS(MS, X) do { \
    static int64_t init = -1; \
    if (init == -1) init = uxr_millis(); \
    if (uxr_millis() - init > MS) { X; init = uxr_millis(); } \
} while (0)




//////////////////////////////////////////////////////////////////////////
// ROS Nodes, Publisher and Subscribers
//////////////////////////////////////////////////////////////////////////

// Onboard-Diag, Test, and Monitoring
// - CPU, RAM, Temperature

// Motor_Control_Node
//  -6x TT Motor
//  -6x MicroServo for steering

// IMU
//

// GPS
//

class MotorControlNode
{
  public:

  private:

};


class GPSNode
{
  public:

  private:

};


class IMUNode
{
public:
    IMUNode() : state_(UNINITIALIZED)
    {
        instance_ = this;
    }

    void setup()
    {
        Wire.begin();
        pinMode(LED_PIN, OUTPUT);
        setup_imu();
    }

    bool create_entities(rclc_support_t* support, rcl_node_t* node, rcl_allocator_t* allocator)
    {
        // Initialize publisher
        RCCHECK(rclc_publisher_init_best_effort(
            &publisher_,
            node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
            "imu_raw"));

        // Timer
        const unsigned int timer_timeout = 100; // 10 Hz
        RCCHECK(rclc_timer_init_default(
            &timer_,
            support,
            RCL_MS_TO_NS(timer_timeout),
            timer_callback));

        return true;
    }

    rcl_timer_t* get_timer()
    {
        return &timer_;
    }

private:
    enum State
    {
        UNINITIALIZED,
        INITIALIZED
    };

    static IMUNode* instance_;

    rcl_publisher_t publisher_;
    rcl_timer_t timer_;
    State state_;

    static void timer_callback(rcl_timer_t* timer, int64_t last_call_time)
    {
        (void)timer;
        (void)last_call_time;
        if (instance_) {
            instance_->publish_imu();
        }
    }

    void setup_imu()
    {
        Wire.beginTransmission(0x68);
        Wire.write(0x6B);  // PWR_MGMT_1 register
        Wire.write(0);     // Set to zero (wakes up MPU-6050)
        Wire.endTransmission(true);
        state_ = INITIALIZED;
    }

    void read_raw_data(int16_t& ax, int16_t& ay, int16_t& az, int16_t& gx, int16_t& gy, int16_t& gz)
    {
        Wire.beginTransmission(0x68);
        Wire.write(0x3B); // Starting register for accelerometer
        Wire.endTransmission(false);
        Wire.requestFrom(0x68, 14, true);

        ax = Wire.read() << 8 | Wire.read();
        ay = Wire.read() << 8 | Wire.read();
        az = Wire.read() << 8 | Wire.read();
        Wire.read(); Wire.read(); // Skip Temp
        gx = Wire.read() << 8 | Wire.read();
        gy = Wire.read() << 8 | Wire.read();
        gz = Wire.read() << 8 | Wire.read();
    }

    void publish_imu()
    {
        if (state_ != INITIALIZED) return;

        int16_t ax, ay, az, gx, gy, gz;
        read_raw_data(ax, ay, az, gx, gy, gz);

        sensor_msgs__msg__Imu msg;
        memset(&msg, 0, sizeof(msg)); // clear unused fields

        // Convert to SI units if needed (optional)
        msg.linear_acceleration.x = ax / 16384.0;  // assuming ±2g
        msg.linear_acceleration.y = ay / 16384.0;
        msg.linear_acceleration.z = az / 16384.0;

        msg.angular_velocity.x = gx / 131.0; // assuming ±250 deg/s
        msg.angular_velocity.y = gy / 131.0;
        msg.angular_velocity.z = gz / 131.0;

        rcl_publish(&publisher_, &msg, nullptr);
    }
};


//////////////////////////////////////////////////////////////////////////
// Test Publisher (Int32Counter)
//////////////////////////////////////////////////////////////////////////

// move the Int32Publisher here and rename previous class to agent_handler

class test_Int32Publisher
{
public:

private:

};


//////////////////////////////////////////////////////////////////////////
// Custom Memory Allocator
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Memory Management
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Time Sync
//////////////////////////////////////////////////////////////////////////

// add the parts of the mROS time_sync_example here

class timesync
{
public:

private:

};

//////////////////////////////////////////////////////////////////////////
// Error-Handling
//////////////////////////////////////////////////////////////////////////

// Control the LED and other events

class error_handler
{
public:

private:

};


//////////////////////////////////////////////////////////////////////////
// ROS Agent Handler, Connection/Reconnection-Routine
//////////////////////////////////////////////////////////////////////////

class AgentHandler
{
public:
    AgentHandler() : msg_{}, state_(WAITING_AGENT)
    {
        instance_ = this;  // Set static instance pointer
    }

    void setup()
    {
        set_microros_transports();
        pinMode(LED_PIN, OUTPUT);
        msg_.data = 0;
    }

    void loop()
    {
        switch (state_)
        {
        case WAITING_AGENT:
            EXECUTE_EVERY_N_MS(500,
                {
                    state_ = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;
                });
            break;
        case AGENT_AVAILABLE:
            state_ = create_entities() ? AGENT_CONNECTED : WAITING_AGENT;
            if (state_ == WAITING_AGENT)
            {
                destroy_entities();
            }
            break;
        case AGENT_CONNECTED:
            EXECUTE_EVERY_N_MS(200,
                {
                    state_ = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;
                });
            if (state_ == AGENT_CONNECTED)
            {
                rclc_executor_spin_some(&executor_, RCL_MS_TO_NS(100));
            }
            break;
        case AGENT_DISCONNECTED:
            destroy_entities();
            state_ = WAITING_AGENT;
            break;
        }

        digitalWrite(LED_PIN, (state_ == AGENT_CONNECTED) ? HIGH : LOW);
    }

private:
    enum State
    {
        WAITING_AGENT,
        AGENT_AVAILABLE,
        AGENT_CONNECTED,
        AGENT_DISCONNECTED
    };

    rclc_support_t support_;
    rcl_node_t node_;
    rcl_timer_t timer_;
    rclc_executor_t executor_;
    rcl_allocator_t allocator_;
    rcl_publisher_t publisher_;
    std_msgs__msg__Int32 msg_;
    State state_;

    static AgentHandler* instance_;  // Static instance pointer

    static void timer_callback(rcl_timer_t* timer, int64_t last_call_time)
    {
        (void)timer;
        (void)last_call_time;
        if (instance_)
        {
            instance_->publish_message();
        }
    }

    void publish_message()
    {
        rcl_publish(&publisher_, &msg_, nullptr);
        msg_.data++;
    }

    bool create_entities()
    {
        allocator_ = rcl_get_default_allocator();

        RCCHECK(rclc_support_init(&support_, 0, nullptr, &allocator_));
        RCCHECK(rclc_node_init_default(&node_, "int32_publisher_rclcpp", "", &support_));
        RCCHECK(rclc_publisher_init_best_effort(
            &publisher_,
            &node_,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
            "std_msgs_msg_Int32"));

        const unsigned int timer_timeout = 1000;
        RCCHECK(rclc_timer_init_default(
            &timer_,
            &support_,
            RCL_MS_TO_NS(timer_timeout),
            timer_callback));

        executor_ = rclc_executor_get_zero_initialized_executor();
        RCCHECK(rclc_executor_init(&executor_, &support_.context, 1, &allocator_));
        RCCHECK(rclc_executor_add_timer(&executor_, &timer_));

        return true;
    }

    void destroy_entities()
    {
        rmw_context_t* rmw_context = rcl_context_get_rmw_context(&support_.context);
        (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

        (void)rcl_publisher_fini(&publisher_, &node_);
        (void)rcl_timer_fini(&timer_);
        rclc_executor_fini(&executor_);
        (void)rcl_node_fini(&node_);
        rclc_support_fini(&support_);
    }
};


//////////////////////////////////////////////////////////////////////////
// Global Scope Definitions
//////////////////////////////////////////////////////////////////////////

// Initialize static member
AgentHandler* AgentHandler::instance_ = nullptr;
AgentHandler publisher_node;


//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////

void setup() {
    publisher_node.setup();
}

void loop() {
    publisher_node.loop();
}
