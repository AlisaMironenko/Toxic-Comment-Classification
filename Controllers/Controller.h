#pragma once

#include<memory>

#include "../DataProviders/DataProvider.h"
#include "../Cores/Core.h"
#include "../DataHandlers/DataPreprocessing.h"


/**
namespace tcc
@brief ������������ ���� tcc
*/
namespace tcc {
	/**
	@brief ��������� ��� ������� ������������ ������������ ����� ���������
	*/
	class Controller {
		/**
		@brief ����������� ������� ��� ������� �������� ���������
		*/
		virtual void run() const = 0;
	};


	/**
	@brief �������� ����� ���������� ����������� ����� ���������
	*/
	class MainController : public Controller {
	private:
		std::shared_ptr<DataProvider> _data_provider;
		std::shared_ptr<Core> _core;
		std::shared_ptr<DataPreprocessing> _data_processing;

	public:
		/**
		@brief ����������� ������
		@param data_provider ��������� �� ������������ ���������
		@param data_processing ��������� �� ������������ �������������� ������
		@param core ��������� �� ������������ ����
		*/
		MainController(std::shared_ptr<DataProvider> data_provider,
			std::shared_ptr<DataPreprocessing> data_processing,
			std::shared_ptr<Core> core)
			: _data_provider(data_provider), _core(core), _data_processing(data_processing)
		{};

		/**
		@brief ������� ��� ������� �������� ���������
		*/
		void run() const override;
	};
}
