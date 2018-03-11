#pragma once

#include "../includes/json.hpp"


using json = nlohmann::json;

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
		virtual void run(const json& msg) const = 0;
	};

	/**
	@brief ����� - ���� ��� ������������� "��������������������" ������
	*/
	class RandomCore : public Core {
		/**
		@brief ������� ������������� "��������������������" ������
		@param msg ��������� ������, ���������� � ���� ����� ���������� �������
		*/
		void run(const json& msg) const override;
	};
}
