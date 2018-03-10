#pragma once

#include "../DataTypes/Message.h"


/**
namespace tcc
@brief ������������ ���� tcc
*/
namespace tcc {
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
}
