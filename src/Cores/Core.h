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
	public:
		/**
		@brief ����������� ������� ������������� "��������������������" ������
		@param msg ��������� ������, ���������� � ���� ����� ���������� �������
		*/
		virtual void run(json& msg) const = 0;
	};

	/**
	@brief ����� - ���� ��� ������������� "��������������������" ������
	*/
	class RandomCore : public Core {
	private:
		double MAX_PERCENT;
	public:
		/**
		@brief ����������� ������
		*/
		RandomCore(double max_percent=100) : MAX_PERCENT(max_percent)
		{};

		/**
		@brief ������� ������������� "��������������������" ������
		@param msg ��������� ������, ���������� � ���� ����� ���������� �������
		*/
		void run(json& msg) const override;
	};
}
