#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "includes/json.hpp"


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
	};

	/**
	@brief ����� ��� ���������� ������������� �������� � ����� Kaggle
	*/
	class KaggleDataProvider : public DataProvider {
	private:
		std::string _input_file;

		//����������� � �����
		static const char s_delim;
		static const char* s_quot;

		//������� ����� � ����������� � ������ � json
		json _sample_to_json(std::vector<std::string>& text, std::vector<bool>& ratings) const;
		//������ id
		std::string _parse_id(std::string& s) const;
		//������ �����������
		bool _parse_text(std::string& line, std::string& dst) const;
		//������ ������
		std::vector<bool> _parse_rating(std::string& line) const;

	public:
		/**
		@brief ����������� ������
		@param input_file ��� ����� ��� ������
		*/
		KaggleDataProvider(std::string& input_file)
			: _input_file(input_file) {};

		/**
		@brief ������� ��� ���������� ������ �� �����
		@return ������ �������� ������, ���������� � ���� ��������� ������
		@throw IOException ���������� ����������� ��� ��������� � ������� �����
		*/
		std::vector<json> get_data() const override;
	};

	/*
	class SQLProvider : public DataProvider {
		Message get_data() const override;
		bool save_data(const Message& msg) const override;
	};
	*/
}
