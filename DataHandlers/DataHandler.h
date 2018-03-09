#pragma once

#include "../DataTypes/Message.h"

/**
@brief ��������� ��� ������� ������������� ������
*/
class DataProcessing {
	/**
	@brief ����������� ������� ��� ������������� ������
	@param msg ��������� ������, ���������� � ���� ����� ���������� �������������
	*/
	virtual void run(const Message& msg) const = 0;
};
