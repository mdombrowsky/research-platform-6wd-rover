//////////////////////////////////////////////////////////////////////////
//file name:
//project name:
//designer:     Michael Dombrowsky
//created on:   06/02/2025
//last modification:  06/07/2020
//version:      0.0.9
//description:
//notes:
//ide:        AVR Studio 6.1.2730 SP2
//compiler      AVR Toolchain 4.8.4.371, gcc 4.8.4
//cpu config:   Atmel SAM3X8E (ARM Cortex-M3)
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Include Header
//////////////////////////////////////////////////////////////////////////

// Basic Header
#include <stdio.h>

// Atmel, AVR

// Arduino
// remove all headers in here when arduino routines are replaced with avr
#include <Wire.h>
//#include <Arduino_FreeRTOS.h> //not working
//#include <TimeLib.h>

// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>

/*
// currently unused
#include <list.h>
#include <stack_macros.h>
#include <mpu_prototypes.h>
#include <deprecated_definitions.h>
#include <queue.h>
#include <mpu_wrappers.h>
#include <FreeRTOSConfig.h>
#include <stream_buffer.h>
#include <message_buffer.h>
#include <croutine.h>
#include <event_groups.h>
#include <portmacro.h>
#include <projdefs.h>
#include <portable.h>
*/

// MicroROS
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>

#include <std_msgs/msg/string.h>
#include <std_msgs/msg/int32.h>
#include <sensor_msgs/msg/imu.h>
//#include <std_msgs/msg/int32_multi_array.h>
#include <rcutils/logging_macros.h>
#include "rosidl_runtime_c/string_functions.h"
#include "rcl_interfaces/msg/log.h"


//////////////////////////////////////////////////////////////////////////
// Macros and Definitions
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// hardware config
//////////////////////////////////////////////////////////////////////////

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



// prios for rosout messages
#ifndef RCL_LOG_SEVERITY_INFO
#define RCL_LOG_SEVERITY_DEBUG 10
#define RCL_LOG_SEVERITY_INFO 20
#define RCL_LOG_SEVERITY_WARN 30
#define RCL_LOG_SEVERITY_ERROR 40
#define RCL_LOG_SEVERITY_FATAL 50
#endif

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




//////////////////////////////////////////////////////////////////////////
// Test Publisher (Int32Publisher )
//////////////////////////////////////////////////////////////////////////

// move the Int32Publisher here and rename previous class to agent_handler


//////////////////////////////////////////////////////////////////////////
// Test Publisher (MinimalPublisher )
//////////////////////////////////////////////////////////////////////////

class MinimalPublisher
{
public:
    MinimalPublisher() : count_(0), state_(UNINITIALIZED)
    {
        instance_ = this;
    }

    bool setup(rclc_support_t* support, rcl_node_t* node, rcl_allocator_t* allocator)
    {
        allocator_ = allocator;

        // Init publisher
        RCCHECK(rclc_publisher_init_best_effort(
            &publisher_,
            node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
            "topic"));

        // Init timer (every 500ms)
        RCCHECK(rclc_timer_init_default(
            &timer_,
            support,
            RCL_MS_TO_NS(500),
            timer_callback));

        state_ = INITIALIZED;
        return true;
    }

    rcl_timer_t* get_timer()
    {
        return (state_ == INITIALIZED) ? &timer_ : nullptr;
    }

private:
    enum State
    {
        UNINITIALIZED,
        INITIALIZED
    };

    static MinimalPublisher* instance_;

    rcl_publisher_t publisher_;
    rcl_timer_t timer_;
    rcl_allocator_t* allocator_;
    State state_;
    int count_;

    static void timer_callback(rcl_timer_t* timer, int64_t last_call_time)
    {
        (void)timer;
        (void)last_call_time;
        if (instance_)
        {
            instance_->publish();
        }
    }

    void publish()
    {
        if (state_ != INITIALIZED) return;

        char buf[64];
        snprintf(buf, sizeof(buf), "Hello from uROS MinimalPublisher %d", count_++);

        std_msgs__msg__String msg;
        std_msgs__msg__String__init(&msg);
        msg.data.data = buf;
        msg.data.size = strlen(buf);
        msg.data.capacity = sizeof(buf);

        rcl_publish(&publisher_, &msg, nullptr);
    }
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

    // Constructor
    AgentHandler() : msg_{}, state_(WAITING_AGENT)
    {
        instance_ = this;  // Set static instance pointer
    }

    // Destructor
    ~AgentHandler()
    {
        destroy_entities();
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
                // spin nodes here
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

    rcl_publisher_t log_publisher_;
    std_msgs__msg__String log_msg_;

    rcl_interfaces__msg__Log rosout_msg_;

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

        publish_log("Test log message number %d", msg_.data);
    }

    void publish_log(const char* format, ...)
    {
        char buffer[128];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        // Assign strings - use rosidl_runtime_c__String__assign to manage memory properly
        rosidl_runtime_c__String__assign(&rosout_msg_.name, "AgentHandler");
        rosidl_runtime_c__String__assign(&rosout_msg_.msg, buffer);
        rosidl_runtime_c__String__assign(&rosout_msg_.file, __FILE__);
        rosidl_runtime_c__String__assign(&rosout_msg_.function, __func__);
        rosout_msg_.line = __LINE__;
        rosout_msg_.level = RCL_LOG_SEVERITY_INFO; // or other severity

        rcl_publish(&log_publisher_, &rosout_msg_, nullptr);
    }

    bool create_entities()
    {
        allocator_ = rcl_get_default_allocator();

        RCCHECK(rclc_support_init(&support_, 0, nullptr, &allocator_));
        RCCHECK(rclc_node_init_default(&node_, "uROS_Agent", "", &support_));       //node name
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

        //for log-publisher
        rcl_publisher_options_t pub_ops = rcl_publisher_get_default_options();
        pub_ops.qos.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
        pub_ops.qos.durability = RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
        rcl_ret_t ret = rcl_publisher_init(
            &log_publisher_,
            &node_,
            ROSIDL_GET_MSG_TYPE_SUPPORT(rcl_interfaces, msg, Log),
            "/rosout",
            &pub_ops);
        if (ret != RCL_RET_OK)
        {
            // handle error
        }
        rosout_msg_.name.data = nullptr;
        rosout_msg_.msg.data = nullptr;
        rosout_msg_.file.data = nullptr;
        rosout_msg_.function.data = nullptr;
        rosout_msg_.line = 0;
        rosout_msg_.level = RCL_LOG_SEVERITY_INFO; // example

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

        (void)rcl_publisher_fini(&log_publisher_, &node_);
    }


};


//////////////////////////////////////////////////////////////////////////
// Global Scope Definitions
//////////////////////////////////////////////////////////////////////////

// Initialize static member
AgentHandler* AgentHandler::instance_ = nullptr;
AgentHandler publisher_node;

MinimalPublisher* MinimalPublisher::instance_ = nullptr;
MinimalPublisher minimal_pub;


//////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////

// Arduino-Style Implementation

void setup()
{
    publisher_node.setup();





}

void loop()
{
    publisher_node.loop();
}

/*
xTaskCreate(
    TaskBlink,       // task function
    "Blink",         // name
    128,             // stack size
    NULL,            // parameters
    2,               // priority higher
    &TaskBlink_Handler
);
*/

/*
xTaskCreate(
    TaskSerial,
    "Serial",
    128,
    NULL,
    1,               // priority lower
    &TaskSerial_Handler
);
*/

/*
int main()
{
    setup();

    while(1)
    {
        loop();
    }
}
*/
