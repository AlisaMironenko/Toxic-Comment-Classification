#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include "../includes/json.hpp"


using json = nlohmann::json;

/**
namespace tcc
@brief ������������ ���� tcc
*/
namespace tcc {
	/**
	@brief ������ ����������, ������������ ���������� ��������� ������ � ������������ �������
	*/
	template <T>
	class DataConsumer {
		/**
		@brief ����������� ������� ��� ������ ������
		@return void
		@throw IOException ����������, ����������� ��� ��������� � ������� �����
		*/
		virtual void flush() const = 0;
		/**
		@brief ����������� ������� ��� ���������� �������� � ������ ��� ��������� ������
		@param result - �������, ������� ����� ��������
		@return void
		*/

		virtual void buff(T&& result) const = 0;
	};

	/**
	@brief ������ ������ ��� ����������� ����������� ������ � ������������ ������� � ����������� ������ � ����
	*/
	template<T>
	class FileDataConsumer : public DataConsumer<T> {
	private:
		std::string _input_file;
		std::vector<T> _buffered_data;
	public:
		/**
		@brief ����������� ������
		@param input_file ��� ����� ��� ������
		*/
		FileDataConsumer(std::string input_file)
			: _input_file(input_file), _buffered_data() {};

		/**
		@brief ������� ��� ���������� �������� � ������� ��� ��������� ������
		@param result �������, ������� ����� �������� 
		@return void
		*/
		void buff(T&& result) const override { _buffered_data.push_back(result); }

		/**
		@brief ������� ��� ������ ������ � ����
		@throw IOException ��� ������ �������� ��� ������ � �����
		*/
		void flush() const override {
			std::ofstream dst;
			dst.open(_input_file);

			for (el : _buffered_data) dst << el << endl;

			dst.close()
		}
	}