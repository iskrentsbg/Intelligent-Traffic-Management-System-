// Use #ifndef and #define guards to prevent multiple inclusion of header files.
#ifndef TEST_CPP_
#define TEST_CPP_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>
#include <vector>
#include <fstream>

/* Node Communication Namespace: */
#include "NodeCommunication/ExternalMessageManager.hpp"
#include "NodeCommunication/ExternalMessageManager.cpp"
#include "NodeCommunication/ESPNOW.hpp"

/* Decision Namespace: */
#include "DecisionManagement/Interface/IFlowable.hpp"
#include "DecisionManagement/IncomingMessageDelegator.hpp"
#include "DecisionManagement/IncomingMessageDelegator.cpp"
#include "DecisionManagement/IncomingDecisionHandler.hpp"
#include "DecisionManagement/IncomingDecisionHandler.cpp"
#include "DecisionManagement/NodeDecisionAcknowledgementHandler.hpp"
#include "DecisionManagement/NodeDecisionAcknowledgementHandler.cpp"
#include "DecisionManagement/NodeDecisionManager.hpp"
#include "DecisionManagement/NodeDecisionManager.cpp"
#include "DecisionManagement/CarCountToPointsConverter.hpp"
#include "DecisionManagement/CarCountToPointsConverter.cpp"
#include "DecisionManagement/OutgoingDecisionHandler.hpp"
#include "DecisionManagement/OutgoingDecisionHandler.cpp"
#include "DecisionManagement/DecisionAckReturner.hpp"
#include "DecisionManagement/DecisionAckReturner.cpp"
#include "DecisionManagement/OutgoingMessageDelegator.hpp"
#include "DecisionManagement/OutgoingMessageDelegator.cpp"
#include "DecisionManagement/NodeInfo.hpp"
#include "DecisionManagement/NodeInfo.cpp"
#include "DecisionManagement/Datatype/Message.hpp"

/* Lanes Namespace: */
#include "Lanes/LaneManager.hpp"
#include "Lanes/LaneManager.cpp"
#include "Lanes/Lane.hpp"
#include "Lanes/Lane.cpp"
#include "Lanes/ButtonSensor.hpp"
#include "Lanes/ButtonSensor.cpp"

/* TrafficLights Namespace */
#include "TrafficLights/TrafficLightManager.hpp"
#include "TrafficLights/TrafficLightManager.cpp"
#include "TrafficLights/CarLaneTrafficLight.hpp"
#include "TrafficLights/CarLaneTrafficLight.cpp"

using namespace DecisionManagement;
using namespace NodeCommunication;
using namespace Lanes;

// Define LightPins and activePins
const int LightPins[3][3] = {
    // Traffic light pins for lane 1
    {14, 12, 13},
    // Traffic light pins for lane 2
    {25, 26, 27},
    // Traffic light pins for lane 3
    {17, 32, 33}};
std::vector<int> activePins{5, 22, 23};

// Compare functions
bool areMessagesEqual(const Message_t &lhs, const Message_t &rhs)
{
    if (lhs.type != rhs.type)
    {
        return false;
    }

    if (memcmp(lhs.arguments, rhs.arguments, ARGUMENTS_MAXLEN) != 0)
    {
        return false;
    }

    if (memcmp(lhs.sourceMAC, rhs.sourceMAC, MAC_SIZE) != 0)
    {
        return false;
    }

    if (memcmp(lhs.destinationMAC, rhs.destinationMAC, MAC_SIZE) != 0)
    {
        return false;
    }

    return true;
}

bool areDecisionsEqual(const DecisionManagement::Decision_t &lhs, const DecisionManagement::Decision_t &rhs)
{
    return lhs.topic == rhs.topic &&
           lhs.fromNodeID == rhs.fromNodeID &&
           lhs.argument == rhs.argument;
}

class MockFlowable : public DecisionManagement::IFlowable
{
private:
    bool isFlowing;

public:
    MockFlowable() : isFlowing(false) {}

    // Implementation of the interface methods
    void handler() override
    {
        if (isFlowing)
        {
            std::cout << "Handling flow..." << std::endl;
            // Implement the handling logic here
        }
        else
        {
            std::cout << "Flow is stopped. Cannot handle." << std::endl;
        }
    }

    void StopFlow() override
    {
        isFlowing = false;
        std::cout << "Flow has been stopped." << std::endl;
    }

    void Flow() override
    {
        isFlowing = true;
        std::cout << "Flow has started." << std::endl;
    }
};

class MockOutgoingExternalMessage : public IOutgoingExternalMessage
{
public:
    std::vector<Message_t> receivedMessages;

    void receiveOutgoingExternalMessage(Message_t message) override
    {
        // Store the received message for later assertions in tests
        receivedMessages.push_back(message);
    }

    // Additional helper methods for testing
    void assertMessageReceived(const Message_t &expectedMessage)
    {
        bool found = false;
        for (const auto &message : receivedMessages)
        {
            if (areMessagesEqual(message, expectedMessage))
            {
                found = true;
                break;
            }
        }
        assert(found);
    }

    void clearReceivedMessages()
    {
        receivedMessages.clear();
    }

    size_t receivedMessageCount() const
    {
        return receivedMessages.size();
    }
};

class MockOutgoingMessage : public DecisionManagement::IOutgoingMessage
{
public:
    // Track received messages for testing purposes
    DecisionManagement::Message_t receivedMessage;

    void receiveMessage(DecisionManagement::Message_t message) override
    {
        receivedMessage = message;
    }
};

class MockIncomingExternalMessage : public DecisionManagement::IIncomingExternalMessage
{
public:
    // Track received messages for testing purposes
    std::vector<DecisionManagement::Message_t> receivedMessages;
    std::vector<DecisionManagement::Decision_t> receivedDecisions;

    void receiveExternalMessage(DecisionManagement::Message_t message) override
    {
        // Emulate behavior similar to IncomingMessageDelegator and IncomingDecisionHandler
        receivedMessages.push_back(message);

        // Delegate the message based on its type
        if (message.type == DecisionManagement::Message_type::Decision)
        {
            // Convert message to decision and store it
            DecisionManagement::Decision_t decision = convertDecisionMsg(message);
            receivedDecisions.push_back(decision);
        }
    }

private:
    DecisionManagement::Decision_t convertDecisionMsg(DecisionManagement::Message_t decisionMsg)
    {
        // Logic to convert Message_t to Decision_t
        DecisionManagement::Decision_t decision;
        // Your conversion logic here, for example:
        return decision;
    }
};

class MockExternalMessageManager : public ExternalMessageManager
{
private:
    MockIncomingExternalMessage mockIMO;
    MockOutgoingExternalMessage mockOutgoingExternalMessage;

public:
    std::queue<Message_t> outgoingMessageQueue;
    Message_t testMessage;
    MockExternalMessageManager()
        : ExternalMessageManager(mockIMO)
    {
        testMessage.type = DecisionManagement::Message_type::Acknowledgement;
    }

    void receiveOutgoingExternalMessage(Message_t outMessage) override
    {
        std::cout << "Mock EMM: received external message\n";
        sendMessage(outMessage);
    }

    int sendMessage(const Message_t &message)
    {
        std::cout << "Mock NCOM: Entry sendMessage\n";
        enqueueMessage(message);
        std::cout << "Mock NCOM: Message enqueued\n";

        Message_t frontMessage = outgoingMessageQueue.front();
        int sendResult = customSendProtocolMessage(frontMessage);

        if (sendResult != 0)
        { // Assuming 0 is success
            std::cout << "Mock NCOM: message failed to send\n";
            std::cout << "Error ID is: " << sendResult << "\n";
            return -1;
        }
        return 0;
    }

    // Method to get the MockOutgoingExternalMessage for assertions
    MockOutgoingExternalMessage &getMockOutgoingExternalMessage()
    {
        return mockOutgoingExternalMessage;
    }

protected:
    void enqueueMessage(const Message_t &message)
    {
        outgoingMessageQueue.push(message);
    }

    int customSendProtocolMessage(const Message_t &message)
    {
        // Instead of sending the message over a network, pass it to MockOutgoingExternalMessage
        mockOutgoingExternalMessage.receiveOutgoingExternalMessage(message);
        // Return success or failure based on the simulated behavior
        return 0;
    }
};

class MockPointsObserver : public DecisionManagement::IIncomingPoints
{
public:
    int lastReceivedPoints = -1;

    void receiveUpdatedPoints(int points) override
    {
        lastReceivedPoints = points;
    }
};

class TestNodeDecisionManager : public NodeDecisionManager
{
private:
    bool incomingDecisionProcessed = false;
    bool decisionAcknowledged = false;

public:
    TestNodeDecisionManager(IOutgoingDecision &IOD, IReturnDecisionAcknowledgement &IRDA)
        : NodeDecisionManager(IOD, IRDA) {}

    void receiveIncomingDecision(Decision_t incomingDecision) override
    {
        incomingDecisionProcessed = true;
        NodeDecisionManager::receiveIncomingDecision(incomingDecision);
    }

    bool hasProcessedIncomingDecision() const
    {
        return incomingDecisionProcessed;
    }

    void confirmDecisionAcknowledgement() override
    {
        decisionAcknowledged = true;
        NodeDecisionManager::confirmDecisionAcknowledgement();
    }

    bool isDecisionAcknowledged() const
    {
        return decisionAcknowledged;
    }

    void resetFlags()
    {
        incomingDecisionProcessed = false;
        decisionAcknowledged = false;
    }
};

// Mock classes for node communication and decision management
class TestDecisionAcknowledgementObserver : public IDecisionAcknowledgementObserver
{
public:
    bool acknowledgementConfirmed = false;

    void confirmDecisionAcknowledgement() override
    {
        acknowledgementConfirmed = true;
    }
};

class TestIncomingDecision : public IIncomingDecision
{
public:
    std::vector<Decision_t> receivedDecisions;

    void receiveIncomingDecision(Decision_t incomingDecision) override
    {
        receivedDecisions.push_back(incomingDecision);
    }
};

class TestOutgoingDecisionHandler : public DecisionManagement::OutgoingDecisionHandler
{
private:
    Decision_t lastDecision;

public:
    TestOutgoingDecisionHandler(DecisionManagement::IOutgoingMessage &outgoingMessage)
        : DecisionManagement::OutgoingDecisionHandler(outgoingMessage) {}

    void receiveOutgoingDecision(Decision_t outgoingDecision) override
    {
        lastDecision = outgoingDecision;
        OutgoingDecisionHandler::receiveOutgoingDecision(outgoingDecision);
    }

    Decision_t getLastDecision() const { return lastDecision; }
};

class TestOutgoingMessageDelegator : public DecisionManagement::OutgoingMessageDelegator
{
private:
    OutgoingDecisionHandler *outgoingDecisionHandler = nullptr;
    DecisionManagement::DecisionAckReturner *decisionAckReturner = nullptr; // Pointer to the base class
    MockOutgoingExternalMessage mockExternalMessage;
    std::vector<Message_t> delegatedMessages;

public:
    // Custom constructor that initializes with the mock object
    TestOutgoingMessageDelegator()
        : DecisionManagement::OutgoingMessageDelegator(mockExternalMessage)
    {
        // Initialize any necessary members
        outgoingDecisionHandler = nullptr;
        decisionAckReturner = nullptr;
    }

    void testDelegateMessage(Message_t message)
    {
        delegatedMessages.push_back(message);
    }

    // Expose additional methods for testing
    void setOutgoingDecisionHandler(OutgoingDecisionHandler *handler)
    {
        outgoingDecisionHandler = handler;
    }

    bool hasDelegatedMessage(const Message_t &message) const
    {
        for (const auto &delegatedMessage : delegatedMessages)
        {
            if (memcmp(&delegatedMessage, &message, sizeof(Message_t)) == 0)
            {
                return true;
            }
        }
        return false;
    }

    void setDecisionAckReturner(DecisionAckReturner *returner)
    {
        decisionAckReturner = returner;
    }
};

class TestDecisionAckReturner : public DecisionManagement::DecisionAckReturner
{
public:
    bool sentAcknowledgement;
    DecisionManagement::Message_type capturedMessageType;
    MockOutgoingMessage mockOutgoingMessage;
    DecisionManagement::Message_t receivedMessage;
    std::vector<DecisionManagement::Decision_t> acknowledgedDecisions; // To track acknowledged decisions

    TestDecisionAckReturner() : DecisionManagement::DecisionAckReturner(mockOutgoingMessage) {}

    DecisionManagement::Message_type getCapturedMessageType() const
    {
        return capturedMessageType;
    }

    using DecisionManagement::DecisionAckReturner::createAckMessageFromDecision;
    using DecisionManagement::DecisionAckReturner::sendAcknowledgementMessage;

    void sendAcknowledgementMessage(Message_t ackMessage) override
    {
        sentAcknowledgement = true;
        capturedMessageType = ackMessage.type;
        DecisionAckReturner::sendAcknowledgementMessage(ackMessage);
    }

    void acknowledgeDecision(const DecisionManagement::Decision_t &decision)
    {
        acknowledgedDecisions.push_back(decision);
    }

    bool isDecisionAcknowledged(const DecisionManagement::Decision_t &decision) const
    {
        for (const auto &ackDecision : acknowledgedDecisions)
        {
            if (areDecisionsEqual(ackDecision, decision))
            {
                return true;
            }
        }
        return false;
    }

    DecisionManagement::Decision_t getLastAcknowledgedDecision() const
    {
        if (!acknowledgedDecisions.empty())
        {
            return acknowledgedDecisions.back();
        }
        return DecisionManagement::Decision_t(); // Return a default decision if none is acknowledged
    }
};

class TestIncomingMessageDelegator : public DecisionManagement::IncomingMessageDelegator
{

public:
    TestIncomingMessageDelegator(DecisionManagement::IncomingDecisionHandler &idh,
                                 DecisionManagement::NodeDecisionAcknowledgementHandler &ndah)
        : DecisionManagement::IncomingMessageDelegator(idh, ndah) {}

    void testDelegateIncomingMessage(Message_t msg)
    {
        delegateIncomingMessage(msg);
    }
};

class TestExternalMessageManager : public NodeCommunication::ExternalMessageManager
{
public:
    using NodeCommunication::ExternalMessageManager::ExternalMessageManager;
};

class TestNodeInfo : public DecisionManagement::NodeInfo
{
public:
    using DecisionManagement::NodeInfo::NodeInfo;
};

class TestIncomingDecisionHandler : public DecisionManagement::IncomingDecisionHandler
{
public:
    TestIncomingDecision testIncomingDecision;

    bool sendDecisionCalled = false;
    Decision_t lastSentDecision;
    bool receiveDecisionMsgCalled = false;
    Message_t lastReceivedMessage;
    bool decisionProcessed = false;

    // Constructor to initialize with the mock object
    TestIncomingDecisionHandler() : DecisionManagement::IncomingDecisionHandler(testIncomingDecision) {}

    void fakeSendDecision(Decision_t newDecision)
    {
        // Flag that the method was called
        sendDecisionCalled = true;
        // Store the decision for later assertion
        lastSentDecision = newDecision;
        testIncomingDecision.receiveIncomingDecision(newDecision);
    }

    // Methods for testing
    Decision_t fakeConvertDecisionMsg(const Message_t &testMessage)
    {
        Decision_t decision;
        std::istringstream ss(std::string(testMessage.arguments, ARGUMENTS_MAXLEN));

        int topic;
        if (!(ss >> topic))
        {
            return Decision_t();
        }
        decision.topic = static_cast<DecisionTopic>(topic);
        // Ignore underscore
        ss.ignore(1);

        if (!(ss >> decision.fromNodeID))
        {
            return Decision_t();
        }
        // Ignore underscore
        ss.ignore(1);

        if (!(ss >> decision.argument))
        {
            return Decision_t();
        }

        return decision;
    }

    void testReceiveDecisionMsg(const Message_t &testMessage)
    {
        // Flag that the method was called
        receiveDecisionMsgCalled = true;
        // Store the message for later assertion
        lastReceivedMessage = testMessage;
        receiveDecisionMsg(testMessage);
        decisionProcessed = true;
    }

    bool hasProcessedDecision() const
    {
        return decisionProcessed;
    }
};

class TestIncomingAckHandler : public DecisionManagement::NodeDecisionAcknowledgementHandler
{
public:
    using DecisionManagement::NodeDecisionAcknowledgementHandler::NodeDecisionAcknowledgementHandler;
};

// Test class for NodeDecisionAcknowledgementHandler
class TestNodeDecisionAcknowledgementHandler : public DecisionManagement::NodeDecisionAcknowledgementHandler
{
    // Allow TestNodeDecisionAcknowledgementHandler to access private members of NodeDecisionAcknowledgementHandler
    friend class TestSetupIncomingMessageDelegator;

public:
    // Constructor that takes a TestDecisionAcknowledgementObserver
    TestNodeDecisionAcknowledgementHandler(TestDecisionAcknowledgementObserver &observer)
        : DecisionManagement::NodeDecisionAcknowledgementHandler(observer) {}

    // Additional member for testing purpose
    bool receiveAckMessageCalled = false;
    Message_t lastReceivedAckMessage;

    // Method to simulate receiving an acknowledgment message
    void testReceiveDecisionAckMessage(const Message_t &incomingMessage)
    {
        receiveAckMessageCalled = true;
        lastReceivedAckMessage = incomingMessage;
        ReceiveDecisionAckMessage(incomingMessage);
    }
};

// LaneManager test classes
class TestLane : public Lanes::Lane
{
public:
    TestLane(int dummy1, int dummy2, int dummy3) : Lanes::Lane(dummy1, dummy2, dummy3)
    {
    }
};

class TestButtonSensor : public Lanes::ButtonSensor
{
public:
    TestButtonSensor(int dummyPin, int dummyParam, TestLane *lane) : Lanes::ButtonSensor(dummyPin, dummyParam, lane)
    {
    }
};

class TestLaneManager : public Lanes::LaneManager
{
public:
    TestLaneManager(std::vector<Lanes::Lane *> *lanes, std::vector<Lanes::ButtonSensor *> *buttons, std::vector<int> activePins, DecisionManagement::IIncomingPoints &pointsObserver, DecisionManagement::IControlsFlow &flowController)
        : Lanes::LaneManager(lanes, buttons, activePins, pointsObserver, flowController) {}
};

// Setup classes test
class TestSetupNodeCommunication : public ::testing::Test
{
protected:
    NodeCommunication::ExternalMessageManager *messageManager;
    TestIncomingMessageDelegator incomingMessageDelegator;

    void SetUp() override
    {
        // Instantiate ExternalMessageManager with any necessary parameters
        messageManager = new NodeCommunication::ExternalMessageManager(incomingMessageDelegator);
    }

    void TearDown() override
    {
        // Cleanup the test object
        delete messageManager;
    }
};

class TestSetupIncomingMessageDelegator : public ::testing::Test
{
protected:
    TestIncomingDecisionHandler testIncomingDecisionHandler;
    TestNodeDecisionAcknowledgementHandler testIncomingAckHandler;
    TestIncomingMessageDelegator testIncomingMessageDelegator;

    void SetUp() override
    {
        // Perform any necessary setup here
    }

    void TearDown() override
    {
        // Perform any necessary cleanup here
    }
};

class MockExternalMessageManagerTest : public ::testing::Test
{
protected:
    MockExternalMessageManager manager;
};

class TestSetupDecisionManagement : public ::testing::Test
{
protected:
    DecisionManagement::NodeInfo *nodeInfo;
    MockOutgoingMessage mockOutgoingMessage;
    TestOutgoingDecisionHandler testOutgoingDecisionHandler;
    TestOutgoingMessageDelegator testOutgoingMessageDelegator;
    TestDecisionAckReturner testDecisionAckReturner;
    TestIncomingDecisionHandler mockDecisionHandler;
    TestDecisionAcknowledgementObserver testObserver;
    TestNodeDecisionAcknowledgementHandler *mockAckHandler;
    TestIncomingMessageDelegator *testDelegator;

public:
    TestSetupDecisionManagement()
        : testOutgoingDecisionHandler(mockOutgoingMessage)
    {
    }

    void SetUp() override
    {
        nodeInfo = new DecisionManagement::NodeInfo();
        mockAckHandler = new TestNodeDecisionAcknowledgementHandler(testObserver);
        testDelegator = new TestIncomingMessageDelegator(mockDecisionHandler, *mockAckHandler);

        testOutgoingMessageDelegator.setOutgoingDecisionHandler(&testOutgoingDecisionHandler);
        testOutgoingMessageDelegator.setDecisionAckReturner(&testDecisionAckReturner);
    }

    void TearDown() override
    {
        delete nodeInfo;
        delete mockAckHandler;
        delete testDelegator;
    }
};

class TestSetupLaneManagement : public ::testing::Test
{
protected:
    TestLane *LaneOne;
    TestLane *LaneTwo;
    TestLane *LaneThree;
    TestButtonSensor *testBtnUp;
    TestButtonSensor *testBtnDown;
    TestButtonSensor *testBtnSwitch;
    TestLaneManager *testLaneManager;
    TestNodeDecisionManager *testNodeDecisionManager;
    TestOutgoingDecisionHandler *testOutgoingDecisionHandler;
    TestOutgoingMessageDelegator *testOutgoingMessageDelegator;
    DecisionAckReturner *decisionAckReturner;

    void SetUp() override
    {
        // Initialize the OutgoingDecisionHandler and DecisionAckReturner
        testOutgoingMessageDelegator = new TestOutgoingMessageDelegator();

        // Initialize OutgoingDecisionHandler with the TestOutgoingMessageDelegator
        testOutgoingDecisionHandler = new TestOutgoingDecisionHandler(*testOutgoingMessageDelegator);
        decisionAckReturner = new DecisionAckReturner(*testOutgoingMessageDelegator);

        // Initialize NodeDecisionManager with actual implementations
        testNodeDecisionManager = new TestNodeDecisionManager(*testOutgoingDecisionHandler, *decisionAckReturner);

        // Initialize lanes with dummy values (or actual values if needed)
        LaneOne = new TestLane(0, 0, 0);
        LaneTwo = new TestLane(0, 0, 0);
        LaneThree = new TestLane(0, 0, 0);

        // Initialize button sensors with dummy values (or actual values if needed)
        testBtnUp = new TestButtonSensor(0, 0, LaneOne);
        testBtnDown = new TestButtonSensor(0, 0, LaneOne);
        testBtnSwitch = new TestButtonSensor(0, 0, LaneOne);

        // Create vectors for lanes and buttons
        std::vector<Lane *> lanes = {LaneOne, LaneTwo, LaneThree};
        std::vector<ButtonSensor *> buttons = {testBtnUp, testBtnDown, testBtnSwitch};
        std::vector<int> activePins = {0, 0, 0};

        // Instantiate TestLaneManager with necessary arguments
        testLaneManager = new TestLaneManager(&lanes, &buttons, activePins, *testNodeDecisionManager, *testNodeDecisionManager);

        // Additional setup if required
    }

    void TearDown() override
    {
        delete testLaneManager;
        delete testNodeDecisionManager;
        delete testOutgoingDecisionHandler;
        delete decisionAckReturner;
        delete testBtnSwitch;
        delete testBtnDown;
        delete testBtnUp;
        delete LaneThree;
        delete LaneTwo;
        delete LaneOne;
    }
};

// Test cases for CarCountToPointsConverter class

class CarCountToPointsConverterTest : public ::testing::Test
{
protected:
    CarCountToPointsConverter *converter;
    MockPointsObserver *mockObserver;

    void SetUp() override
    {
        converter = new CarCountToPointsConverter();
        mockObserver = new MockPointsObserver();
        converter->addPointObserver(mockObserver);
    }

    void TearDown() override
    {
        delete mockObserver;
        delete converter;
    }
};

TEST_F(CarCountToPointsConverterTest, TypicalUseCase)
{
    int expectedPointsFor5Cars = 5;
    int actualPoints = converter->calculatePointsFromCarCount(5);
    ASSERT_EQ(expectedPointsFor5Cars, actualPoints);
}

TEST_F(CarCountToPointsConverterTest, BoundaryCondition)
{
    MockPointsObserver mockObserver;
    converter->addPointObserver(&mockObserver);
    converter->newLocalCarCount(100);
    ASSERT_EQ(100, mockObserver.lastReceivedPoints);
}

TEST_F(CarCountToPointsConverterTest, EdgeCaseZeroCars)
{
    MockPointsObserver mockObserver;
    converter->addPointObserver(&mockObserver);
    converter->newLocalCarCount(0);
    ASSERT_EQ(0, mockObserver.lastReceivedPoints);
}

TEST_F(CarCountToPointsConverterTest, NegativeCarCount)
{
    MockPointsObserver mockObserver;
    converter->addPointObserver(&mockObserver);
    converter->newLocalCarCount(-1);

    ASSERT_EQ(0, mockObserver.lastReceivedPoints);
}

TEST_F(CarCountToPointsConverterTest, ObserverAdditionAndRemoval)
{
    MockPointsObserver observer1, observer2;
    converter->addPointObserver(&observer1);
    converter->addPointObserver(&observer2);
    converter->newLocalCarCount(3);
    ASSERT_EQ(3, observer1.lastReceivedPoints);
    ASSERT_EQ(3, observer2.lastReceivedPoints);

    converter->removePointObserver(&observer1);
    converter->newLocalCarCount(6);
    // observer1 should not update
    ASSERT_EQ(3, observer1.lastReceivedPoints);
    // observer2 should update
    ASSERT_EQ(6, observer2.lastReceivedPoints);
}

// Test cases for DecisionAckReturner class

TEST(TestDecisionAckReturner, CreatingAckMessageFromDecision)
{
    TestDecisionAckReturner testAckReturner;

    // Test decision
    DecisionManagement::Decision_t testDecision;
    // Set the fromNodeID
    testDecision.fromNodeID = 1;

    // Call the protected method createAckMessageFromDecision
    DecisionManagement::Message_t ackMessage = testAckReturner.createAckMessageFromDecision(testDecision);

    // Perform assertions on ackMessage
    ASSERT_EQ(ackMessage.type, DecisionManagement::Message_type::Acknowledgement);
}

TEST(TestDecisionAckReturner, SendingAcknowledgementMessage)
{
    // Create an instance of TestDecisionAckReturner
    TestDecisionAckReturner testAckReturner;

    // Create a test acknowledgment message
    Message_t testAckMessage;

    // Call the protected method sendAcknowledgementMessage
    testAckReturner.sendAcknowledgementMessage(testAckMessage);

    // Perform assertions
    ASSERT_TRUE(testAckReturner.sentAcknowledgement);
}

TEST(TestDecisionAckReturner, ReceivingDecisionToAcknowledge)
{
    // Create an instance of TestDecisionAckReturner
    TestDecisionAckReturner testAckReturner;

    // Creating a test decision to acknowledge
    Decision_t testDecisionToAck;
    testDecisionToAck.fromNodeID = 2;

    // Calling the public method receiveDecisionToAcknowledge
    testAckReturner.receiveDecisionToAcknowledge(testDecisionToAck);

    // Asserting that the sendAcknowledgementMessage function was called
    // This ensures that the expected action is taken upon receiving the decision
    ASSERT_TRUE(testAckReturner.sentAcknowledgement);

    // Get the captured AckMessage type using the new method
    DecisionManagement::Message_type capturedMessageType = testAckReturner.getCapturedMessageType();

    // Asserting that the captured message type is as expected
    ASSERT_EQ(capturedMessageType, DecisionManagement::Message_type::Acknowledgement);
}

// Test cases for TestIncomingDecisionHandler class
TEST(TestIncomingDecisionHandler, ConvertingValidDecisionMessage)
{
    TestIncomingDecisionHandler handler;

    // Prepare a valid Decision_t object
    Decision_t expectedDecision;
    expectedDecision.topic = DecisionTopic::NEWPOINTS;
    expectedDecision.fromNodeID = 123;
    expectedDecision.argument = 456;

    // Serialize expectedDecision into a string
    std::ostringstream oss;
    oss << static_cast<int>(expectedDecision.topic) << "_"
        << expectedDecision.fromNodeID << "_"
        << expectedDecision.argument;

    // Set up the validMessage with the serialized data
    Message_t validMessage;
    validMessage.type = Message_type::Decision;
    strncpy(validMessage.arguments, oss.str().c_str(), ARGUMENTS_MAXLEN);
    std::fill_n(validMessage.sourceMAC, MAC_SIZE, 0x01);
    std::fill_n(validMessage.destinationMAC, MAC_SIZE, 0x02);

    // Deserialize and compare
    Decision_t actualDecision = handler.fakeConvertDecisionMsg(validMessage);

    ASSERT_EQ(actualDecision.topic, expectedDecision.topic);
    ASSERT_EQ(actualDecision.fromNodeID, expectedDecision.fromNodeID);
    ASSERT_EQ(actualDecision.argument, expectedDecision.argument);
}

TEST(TestIncomingDecisionHandler, ConvertingInvalidDecisionMessage)
{
    TestIncomingDecisionHandler handler;
    Message_t invalidMessage;
    invalidMessage.type = Message_type::Decision;
    strncpy(invalidMessage.arguments, "Invalid_Data", ARGUMENTS_MAXLEN);

    Decision_t result = handler.fakeConvertDecisionMsg(invalidMessage);

    Decision_t defaultDecision;
    defaultDecision.topic = static_cast<DecisionTopic>(0);
    defaultDecision.fromNodeID = 0;
    defaultDecision.argument = 0;

    ASSERT_EQ(result.topic, defaultDecision.topic);
    ASSERT_EQ(result.fromNodeID, defaultDecision.fromNodeID);
    ASSERT_EQ(result.argument, defaultDecision.argument);
}

TEST(TestIncomingDecisionHandler, FakingSendingDecision)
{
    TestIncomingDecisionHandler handler;
    // Initialize expectedDecision
    Decision_t expectedDecision;
    expectedDecision.topic = DecisionTopic::NEWPOINTS;
    expectedDecision.fromNodeID = 42;
    expectedDecision.argument = 100;

    handler.fakeSendDecision(expectedDecision);

    ASSERT_TRUE(handler.sendDecisionCalled);
    ASSERT_EQ(handler.lastSentDecision.topic, expectedDecision.topic);
    ASSERT_EQ(handler.lastSentDecision.fromNodeID, expectedDecision.fromNodeID);
    ASSERT_EQ(handler.lastSentDecision.argument, expectedDecision.argument);
}

TEST(TestIncomingDecisionHandler, ReceivingDecisionMessage)
{
    TestIncomingDecisionHandler handler;

    // Initialize expectedMessage
    Message_t expectedMessage;
    expectedMessage.type = Message_type::Decision;
    std::string arguments = "1_42_100";
    strncpy(expectedMessage.arguments, arguments.c_str(), ARGUMENTS_MAXLEN);
    std::fill_n(expectedMessage.sourceMAC, MAC_SIZE, 0x01);
    std::fill_n(expectedMessage.destinationMAC, MAC_SIZE, 0x02);

    handler.testReceiveDecisionMsg(expectedMessage);

    ASSERT_TRUE(handler.receiveDecisionMsgCalled);
    ASSERT_EQ(handler.lastReceivedMessage.type, expectedMessage.type);

    // Compare arguments field
    for (int i = 0; i < ARGUMENTS_MAXLEN; ++i)
    {
        ASSERT_EQ(handler.lastReceivedMessage.arguments[i], expectedMessage.arguments[i]);
    }

    // Compare sourceMAC field
    for (int i = 0; i < MAC_SIZE; ++i)
    {
        ASSERT_EQ(handler.lastReceivedMessage.sourceMAC[i], expectedMessage.sourceMAC[i]);
    }

    // Compare destinationMAC field
    for (int i = 0; i < MAC_SIZE; ++i)
    {
        ASSERT_EQ(handler.lastReceivedMessage.destinationMAC[i], expectedMessage.destinationMAC[i]);
    }
}

// Test cases for IncomingMessageDelegator class
TEST(TestIncomingMessageDelegator, DelegatingDecisionMessage)
{
    // Create mock handlers and the delegator
    TestIncomingDecisionHandler mockDecisionHandler;
    TestDecisionAcknowledgementObserver testObserver;
    TestNodeDecisionAcknowledgementHandler mockAckHandler(testObserver);
    TestIncomingMessageDelegator testDelegator(mockDecisionHandler, mockAckHandler);

    // Create a test message of type Decision
    Message_t testMessage;
    testMessage.type = Message_type::Decision;

    // Delegate the test message to the delegator
    testDelegator.testDelegateIncomingMessage(testMessage);

    // Calling the test-specific method to simulate receiving an acknowledgment message
    mockDecisionHandler.testReceiveDecisionMsg(testMessage);

    // Ensure the mockDecisionHandler received the message and it's equal to the test message
    ASSERT_TRUE(mockDecisionHandler.receiveDecisionMsgCalled);
    ASSERT_TRUE(areMessagesEqual(testMessage, mockDecisionHandler.lastReceivedMessage));
}

TEST(TestIncomingMessageDelegator, DelegatingAcknowledgementMessage)
{
    // Create mock handlers and the delegator
    TestIncomingDecisionHandler mockDecisionHandler;
    TestDecisionAcknowledgementObserver testObserver;
    TestNodeDecisionAcknowledgementHandler mockAckHandler(testObserver);
    TestIncomingMessageDelegator testDelegator(mockDecisionHandler, mockAckHandler);

    // Create a test message of type Acknowledgement
    Message_t testMessage;
    testMessage.type = Message_type::Acknowledgement;

    // Delegate the test message to the delegator
    testDelegator.testDelegateIncomingMessage(testMessage);

    // Calling the test-specific method to simulate receiving an acknowledgment message
    mockAckHandler.testReceiveDecisionAckMessage(testMessage);

    // Ensure the mockAckHandler received the acknowledgement message and it's equal to the test message
    ASSERT_TRUE(mockAckHandler.receiveAckMessageCalled);
    ASSERT_TRUE(areMessagesEqual(testMessage, mockAckHandler.lastReceivedAckMessage));
}

TEST(TestIncomingMessageDelegator, DelegatingDefaultCase)
{
    // Create mock handlers and the delegator
    TestIncomingDecisionHandler mockDecisionHandler;
    TestDecisionAcknowledgementObserver testObserver;
    TestNodeDecisionAcknowledgementHandler mockAckHandler(testObserver);
    TestIncomingMessageDelegator testDelegator(mockDecisionHandler, mockAckHandler);

    // Create a test message with an unrecognized type
    Message_t testMessage;
    testMessage.type = static_cast<Message_type>(999);

    // Delegate the test message to the delegator
    testDelegator.testDelegateIncomingMessage(testMessage);

    // Ensure neither mock handler received the message
    ASSERT_FALSE(mockDecisionHandler.receiveDecisionMsgCalled);
    ASSERT_FALSE(mockAckHandler.receiveAckMessageCalled);
}

TEST(TestNodeDecisionAcknowledgementHandler, ReceivingDecisionAckMessage)
{
    // Create an instance of TestDecisionAcknowledgementObserver
    TestDecisionAcknowledgementObserver testObserver;

    // Create an instance of TestNodeDecisionAcknowledgementHandler
    TestNodeDecisionAcknowledgementHandler handler(testObserver);
    // Initialize expectedMessage
    Message_t expectedMessage;
    expectedMessage.type = Message_type::Acknowledgement;
    std::string arguments = "1_42_100";
    strncpy(expectedMessage.arguments, arguments.c_str(), ARGUMENTS_MAXLEN);
    std::fill_n(expectedMessage.sourceMAC, MAC_SIZE, 0x01);
    std::fill_n(expectedMessage.destinationMAC, MAC_SIZE, 0x02);

    handler.testReceiveDecisionAckMessage(expectedMessage);

    ASSERT_TRUE(handler.receiveAckMessageCalled);
    ASSERT_EQ(handler.lastReceivedAckMessage.type, expectedMessage.type);

    // Compare arguments field
    for (int i = 0; i < ARGUMENTS_MAXLEN; ++i)
    {
        ASSERT_EQ(handler.lastReceivedAckMessage.arguments[i], expectedMessage.arguments[i]);
    }

    // Compare sourceMAC field
    for (int i = 0; i < MAC_SIZE; ++i)
    {
        ASSERT_EQ(handler.lastReceivedAckMessage.sourceMAC[i], expectedMessage.sourceMAC[i]);
    }

    // Compare destinationMAC field
    for (int i = 0; i < MAC_SIZE; ++i)
    {
        ASSERT_EQ(handler.lastReceivedAckMessage.destinationMAC[i], expectedMessage.destinationMAC[i]);
    }
}

// Test cases for IncomingMessageDelegator class
TEST(TestIncomingMessageDelegator, DelegatingDecisionAckMessage)
{
    // Create mock handlers and the delegator
    TestIncomingDecisionHandler mockDecisionHandler;
    TestDecisionAcknowledgementObserver testObserver;
    TestNodeDecisionAcknowledgementHandler mockAckHandler(testObserver);
    TestIncomingMessageDelegator testDelegator(mockDecisionHandler, mockAckHandler);

    // Create a test message of type Acknowledgement
    Message_t testMessage;
    testMessage.type = Message_type::Acknowledgement;

    // Delegate the test message to the delegator
    testDelegator.testDelegateIncomingMessage(testMessage);

    // Calling the test-specific method to simulate receiving an acknowledgment message
    mockAckHandler.testReceiveDecisionAckMessage(testMessage);

    // Ensure the mockAckHandler received the acknowledgment message and it's equal to the test message
    ASSERT_TRUE(mockAckHandler.receiveAckMessageCalled);
    ASSERT_TRUE(areMessagesEqual(testMessage, mockAckHandler.lastReceivedAckMessage));
}

TEST(TestNodeDecisionAcknowledgementHandler, ReceivingDecisionAckMessage_InvalidInput)
{
    // Create an instance of TestDecisionAcknowledgementObserver
    TestDecisionAcknowledgementObserver testObserver;

    // Create an instance of TestNodeDecisionAcknowledgementHandler
    TestNodeDecisionAcknowledgementHandler handler(testObserver);

    // Initialize an invalid acknowledgment message
    Message_t invalidMessage;
    invalidMessage.type = Message_type::Acknowledgement;
    strncpy(invalidMessage.arguments, "Invalid_Data", ARGUMENTS_MAXLEN);

    handler.testReceiveDecisionAckMessage(invalidMessage);

    // Ensure that receiveAckMessageCalled is set to true, indicating that the method was called
    ASSERT_TRUE(handler.receiveAckMessageCalled);

    // Verify that the lastReceivedAckMessage has the expected type (Acknowledgement)
    ASSERT_EQ(handler.lastReceivedAckMessage.type, invalidMessage.type);

    // For an invalid message, the content of lastReceivedAckMessage may not be well-defined,
    // so we won't perform detailed content comparisons in this case.
}

TEST(TestNodeDecisionAcknowledgementHandler, ReceivingMultipleDecisionAckMessages)
{
    // Create an instance of TestDecisionAcknowledgementObserver
    TestDecisionAcknowledgementObserver testObserver;

    // Create an instance of TestNodeDecisionAcknowledgementHandler
    TestNodeDecisionAcknowledgementHandler handler(testObserver);

    // Initialize multiple acknowledgment messages
    for (int i = 0; i < 5; ++i)
    {
        Message_t ackMessage;
        ackMessage.type = Message_type::Acknowledgement;
        // Set unique arguments for each message
        std::string arguments = std::to_string(i) + "_42_100";
        strncpy(ackMessage.arguments, arguments.c_str(), ARGUMENTS_MAXLEN);

        handler.testReceiveDecisionAckMessage(ackMessage);

        // Verify that each received acknowledgment message matches the expected one
        ASSERT_TRUE(handler.receiveAckMessageCalled);
        ASSERT_EQ(handler.lastReceivedAckMessage.type, ackMessage.type);

        // Check the content of the received acknowledgment message
        ASSERT_TRUE(areMessagesEqual(handler.lastReceivedAckMessage, ackMessage));

        // Reset flags for the next iteration
        handler.receiveAckMessageCalled = false;
    }
}

TEST_F(TestSetupLaneManagement, ReceiveUpdatedPoints_TriggerCorrectFlow)
{
    const int testPoints = 10;

    // First, trigger the normal flow through receiveUpdatedPoints
    testNodeDecisionManager->receiveUpdatedPoints(testPoints);

    // Now, create a mock decision to directly test receiveOutgoingDecision
    Decision_t mockDecision;
    mockDecision.topic = NEWPOINTS;
    mockDecision.fromNodeID = NodeInfo::getCurrentNodeID();
    mockDecision.argument = testPoints;

    // Call receiveOutgoingDecision directly with the mock decision
    testOutgoingDecisionHandler->receiveOutgoingDecision(mockDecision);

    // Check if the testOutgoingDecisionHandler is not null
    if (testOutgoingDecisionHandler != nullptr)
    {
        Decision_t lastDecision = testOutgoingDecisionHandler->getLastDecision();
        std::ofstream debugFile("debug_handler_output.txt");
        if (debugFile.is_open())
        {
            debugFile << "Last Decision - Topic: " << lastDecision.topic
                      << ", FromNodeID: " << lastDecision.fromNodeID
                      << ", Argument: " << lastDecision.argument << "\n";
            debugFile.close();
        }
        else
        {
            FAIL() << "Failed to open debug file for writing.";
        }

        // Assert that the last decision matches the expected values
        ASSERT_EQ(lastDecision.topic, NEWPOINTS);
        ASSERT_EQ(lastDecision.fromNodeID, NodeInfo::getCurrentNodeID());
        ASSERT_EQ(lastDecision.argument, testPoints);
    }
    else
    {
        FAIL() << "TestOutgoingDecisionHandler is null";
    }
}

TEST_F(TestSetupLaneManagement, FlowHasStopped_UpdateAndNotifyCorrectly)
{
    testNodeDecisionManager->flowHasStopped();

    // Create a mock decision to directly test receiveOutgoingDecision
    Decision_t mockDecision;
    mockDecision.topic = STOPPEDNOTIFY;
    mockDecision.fromNodeID = NodeInfo::getCurrentNodeID();
    mockDecision.argument = 0; // Assuming argument is 0 for STOPPEDNOTIFY

    // Call receiveOutgoingDecision directly with the mock decision
    testOutgoingDecisionHandler->receiveOutgoingDecision(mockDecision);

    if (testOutgoingDecisionHandler != nullptr)
    {
        Decision_t lastDecision = testOutgoingDecisionHandler->getLastDecision();

        ASSERT_EQ(lastDecision.topic, STOPPEDNOTIFY);
        ASSERT_EQ(lastDecision.fromNodeID, NodeInfo::getCurrentNodeID());
        ASSERT_EQ(lastDecision.argument, 0);
    }
    else
    {
        FAIL() << "TestOutgoingDecisionHandler is null";
    }
}

TEST_F(TestSetupLaneManagement, ConfirmDecisionAcknowledgement_ProcessPendingDecision)
{

    testNodeDecisionManager->confirmDecisionAcknowledgement();
    ASSERT_TRUE(testNodeDecisionManager->isDecisionAcknowledged());
}

TEST_F(TestSetupLaneManagement, ReceiveIncomingDecision_HandleDecisionCorrectly)
{
    Decision_t mockDecision = {NEWPOINTS, 1, 15};
    testNodeDecisionManager->receiveIncomingDecision(mockDecision);
    ASSERT_TRUE(testNodeDecisionManager->hasProcessedIncomingDecision());
}

TEST_F(TestSetupLaneManagement, AddRemoveFlowableObserver_ManageObserversCorrectly)
{
    // Create a mock flowable observer
    MockFlowable *mockFlowableObserver = new MockFlowable();

    // Use mockFlowableObserver without the Lanes:: namespace
    testNodeDecisionManager->addFlowableObserver(mockFlowableObserver);

    // Use mockFlowableObserver without the Lanes:: namespace
    testNodeDecisionManager->removeFlowableObserver(mockFlowableObserver);

    delete mockFlowableObserver;
}

TEST_F(TestSetupDecisionManagement, InitializationAndTeardown)
{
    ASSERT_NE(nodeInfo, nullptr);
}

TEST_F(TestSetupDecisionManagement, ReceiveOutgoingDecision)
{
    const int testPoints = 10;
    Decision_t mockDecision;
    mockDecision.topic = NEWPOINTS;
    mockDecision.fromNodeID = nodeInfo->getCurrentNodeID();
    mockDecision.argument = testPoints;

    testOutgoingDecisionHandler.receiveOutgoingDecision(mockDecision);

    Decision_t lastDecision = testOutgoingDecisionHandler.getLastDecision();
    ASSERT_EQ(lastDecision.topic, NEWPOINTS);
    ASSERT_EQ(lastDecision.fromNodeID, nodeInfo->getCurrentNodeID());
    ASSERT_EQ(lastDecision.argument, testPoints);
}

TEST_F(TestSetupDecisionManagement, ReceiveIncomingDecision)
{
    Message_t decisionMessage;
    decisionMessage.type = Message_type::Decision;
    snprintf(decisionMessage.arguments, ARGUMENTS_MAXLEN, "%d_%d_%d", NEWPOINTS, 1, 15);
    mockDecisionHandler.testReceiveDecisionMsg(decisionMessage);

    ASSERT_TRUE(mockDecisionHandler.hasProcessedDecision());
}

TEST_F(TestSetupDecisionManagement, AcknowledgeDecision)
{
    Decision_t mockDecision = {NEWPOINTS, 1, 15};
    testDecisionAckReturner.acknowledgeDecision(mockDecision);

    Decision_t lastAcknowledgedDecision = testDecisionAckReturner.getLastAcknowledgedDecision();
    ASSERT_TRUE(areDecisionsEqual(mockDecision, lastAcknowledgedDecision));
}

TEST_F(TestSetupDecisionManagement, OutgoingMessageDelegation)
{
    // Create a mock message
    Message_t mockMessage;
    // Initialize mockMessage appropriately
    testOutgoingMessageDelegator.testDelegateMessage(mockMessage);

    ASSERT_TRUE(testOutgoingMessageDelegator.hasDelegatedMessage(mockMessage));
}

TEST_F(TestSetupDecisionManagement, IncomingMessageHandling)
{
    std::ofstream debugFile("debug.txt");
    Message_t mockMessage;
    mockMessage.type = Message_type::Decision;

    mockDecisionHandler.testReceiveDecisionMsg(mockMessage);
    testDelegator->receiveExternalMessage(mockMessage);

    ASSERT_TRUE(mockDecisionHandler.receiveDecisionMsgCalled);
}

TEST(MockExternalMessageManager, MessageReceptionAndEnqueueing)
{
    MockExternalMessageManager manager;
    manager.receiveOutgoingExternalMessage(manager.testMessage);

    // Check if message was enqueued
    ASSERT_FALSE(manager.outgoingMessageQueue.empty());
    Message_t frontMessage = manager.outgoingMessageQueue.front();
    ASSERT_TRUE(areMessagesEqual(frontMessage, manager.testMessage));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // TEST_CPP_