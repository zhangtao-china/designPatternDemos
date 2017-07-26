#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace std;

// 电灯接收者类
class Light
{
public:
	void on()
	{
		cout << "light is on" << endl;
	}

	void off()
	{
		cout << "light is off" << endl;
	}
};

// 音响接收者类
class Stereo
{
public:
	void on()
	{
		cout << "stereo is on" << endl;
	}

	void off()
	{
		cout << "stereo is off" << endl;
	}

	void setCD()
	{
		cout << "stereo is set for CD input" << endl;
	}

	void setVolume(int volume)
	{
		cout << "stereo volume set to " << volume << endl;
	}
};

// 抽象命令类
class Command
{
public:
	virtual void execute() = 0;
	virtual string className() = 0;
};

// 打开电灯命令类
class LightOnCommand : public Command
{
public:
	LightOnCommand(shared_ptr<Light> light)
		: m_light(light)
	{
		if (!light)
		{
			throw invalid_argument("[LightOnCommand::LightOnCommand] light is null.");
		}
	}

	void execute() override
	{
		m_light->on();
	}

	string className() override
	{
		return "LightOnCommand";
	}

private:
	shared_ptr<Light> m_light;
};

// 关闭电灯命令类
class LightOffCommand : public Command
{
public:
	LightOffCommand(shared_ptr<Light> light)
		: m_light(light)
	{
		if (!light)
		{
			throw invalid_argument("[LightOffCommand::LightOffCommand] light is null.");
		}
	}

	void execute() override
	{
		m_light->off();
	}

	string className() override
	{
		return "LightOffCommand";
	}

private:
	shared_ptr<Light> m_light;
};

// 打开音响命令类
class StereoOnWithCDCommand : public Command
{
public:
	explicit StereoOnWithCDCommand(shared_ptr<Stereo> stereo)
		: m_stereo(stereo)
	{
		if(!stereo)
		{
			throw invalid_argument("[StereoOnWithCDCommand::StereoOnWithCDCommand] stereo is null.");
		}
	}

	void execute() override
	{
		m_stereo->on();
		m_stereo->setCD();
		m_stereo->setVolume(5);
	}

	string className() override
	{
		return "StereoOnWithCDCommand";
	}

private:
	shared_ptr<Stereo> m_stereo;
};

// 关闭音响命令类
class StereoOffCommand : public Command
{
public:
	explicit StereoOffCommand(shared_ptr<Stereo> stereo)
		: m_stereo(stereo)
	{
		if (!stereo)
		{
			throw invalid_argument("[StereoOffCommand::StereoOffCommand] stereo is null.");
		}
	}

	void execute() override
	{
		m_stereo->off();
	}

	string className() override
	{
		return "StereoOffCommand";
	}

private:
	shared_ptr<Stereo> m_stereo;
};

// 空命令类
class EmptyCommand : public Command
{
public:
	void execute() override
	{
		cout << "[EmptyCommand::execute]this is empty command, do nothing." << endl;
	}

	string className() override
	{
		return "EmptyCommand";
	}
};

// 调用者类
class RemoteControl
{
public:
	enum Slot
	{
		Slot1 = 0,
		Slot2,

		NSlot
	};
	RemoteControl()
	{
		auto emptyCmd = make_shared<EmptyCommand>();
		m_onCommands.assign(NSlot, emptyCmd);
		m_offCommands.assign(NSlot, emptyCmd);
	}

	void print() const
	{
		cout << "--------------Rmote Control--------------" << endl;
		for (decltype(m_onCommands.size()) i = 0; i < m_onCommands.size(); ++i)
		{
			cout << "[slot " << i << "]" << '\t' << m_onCommands[i]->className() << '\t' << m_offCommands[i]->className() << "\n";
		}

		cout << endl;
	}

	void setCommand(int slot, shared_ptr<Command> onCommand, shared_ptr<Command> offCommand)
	{
		check(slot);

		m_onCommands[slot] = onCommand;
		m_offCommands[slot] = offCommand;
	}

	void onButtonWasPushed(int slot)
	{
		check(slot);
		cout << "slot " << slot << " on button was pushed" << endl;
		m_onCommands[slot]->execute();
	}

	void offButtonWasPushed(int slot)
	{
		check(slot);
		cout << "slot " << slot << " off button was pushed" << endl;
		m_offCommands[slot]->execute();
	}

private:
	void check(int slot)
	{
		if (slot < 0 || slot >= NSlot)
		{
			ostringstream oss;
			oss << "slot " << slot << " is not exist.";
			throw invalid_argument(oss.str());
		}
	}

private:
	vector<shared_ptr<Command>> m_onCommands;
	vector<shared_ptr<Command>> m_offCommands;
};

int main()
{
	// 客户代码
	auto light = make_shared<Light>();
	auto stereo = make_shared<Stereo>();

	auto lightOnCmd = make_shared<LightOnCommand>(light);
	auto lightOffCmd = make_shared<LightOffCommand>(light);

	auto stereoOnCmd = make_shared<StereoOnWithCDCommand>(stereo);
	auto stereoOffCmd = make_shared<StereoOffCommand>(stereo);

	RemoteControl remoteControl;
	remoteControl.setCommand(RemoteControl::Slot1, lightOnCmd, lightOffCmd);
	remoteControl.setCommand(RemoteControl::Slot2, stereoOnCmd, stereoOffCmd);

	remoteControl.print();

	remoteControl.onButtonWasPushed(RemoteControl::Slot1);
	remoteControl.offButtonWasPushed(RemoteControl::Slot1);
	remoteControl.onButtonWasPushed(RemoteControl::Slot2);
	remoteControl.offButtonWasPushed(RemoteControl::Slot2);

	return 0;
}