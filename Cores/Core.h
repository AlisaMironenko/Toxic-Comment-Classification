#pragma once

#include "../DataTypes/Message.h"


/**
namespace tcc
@brief ������������ ���� tcc
*/
namespace tcc {
	/**
	@brief ��������� ������� ���� ��� ������������� "��������������������" ������
	 */
	class Core {
		/**
		@brief ����������� ������� ������������� "��������������������" ������
		@param msg ��������� ������, ���������� � ���� ����� ���������� �������
		*/
		virtual void run(const Message& msg) const = 0;
	};

	/**
	@brief ����� - ���� ��� ������������� "��������������������" ������
	*/
	class RandomCore : public Core {
		/**
		@brief ������� ������������� "��������������������" ������
		@param msg ��������� ������, ���������� � ���� ����� ���������� �������
		*/
		void run(const Message& msg) const override;
	};
}
