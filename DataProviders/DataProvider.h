#pragma once

#include <vector>
#include <string>

#include "../includes/json.hpp"


using json = nlohmann::json;

/**
namespace tcc
@brief ������������ ���� tcc
*/
namespace tcc {
	/**
	@brief ��������� ��� ������� ����������/������ ������
	*/
	class DataProvider {
		/**
		@brief ����������� ������� ��� ���������� ������
		@return ������ �������� ������, ���������� � ���� ��������� ������
		@throw IOException ���������� ����������� ��� ��������� � ������� �����
		*/
		virtual std::vector<json> get_data() const = 0;
		/**
		@brief ����������� ������� ��� ������ ������
		@param msg ��������� ������, ���������� � ���� ������ ���������� ������
		@return ������� ��������:
			- 0 ������ �� ��������
			- 1 ������ �������
		*/
		virtual bool save_data(const json& msg) const = 0;
	};

	/**
	@brief ����� ��� ����������/������ ������ ��/� ����(�)
	*/
	class FileDataProvider : public DataProvider {
	private:
		std::string _input_file;
		std::string _output_file;

	public:
		/**
		@brief ����������� ������
		@param input_file ��� ����� ��� ������
		@param output_file ��� ����� ��� ������
		*/
		FileDataProvider(std::string input_file, std::string output_file)
			: _input_file(input_file), _output_file(output_file)
		{};

		/**
		@brief ������� ��� ���������� ������ �� �����
		@return ������ �������� ������, ���������� � ���� ��������� ������
		@throw IOException ���������� ����������� ��� ��������� � ������� �����
		*/
		std::vector<json> get_data() const override;

		/**
		@brief ������� ��� ������ ������ � ����
		@param msg ��������� ������, ���������� � ���� ������ ���������� ������
		@return ������� ��������:
			- 0 ������ �� ��������
			- 1 ������ �������
		*/
		bool save_data(const json& msg) const override;
	};

	/*
	class SQLProvider : public DataProvider {
		Message get_data() const override;
		bool save_data(const Message& msg) const override;
	};
	*/
}
