#include "includes/json.hpp"
#include "Classification/Classifyer.h"

#define LABELS_COUNT 5


using json = nlohmann::json;
using textVec = std::vector<bool>;
using labels = std::array<bool, 5>;
using labeledText = std::vector<std::pair<textVec, labels>>;

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
		@param t �����
		*/
		virtual std::vector<double> run(textVec& t) const = 0;
		/**
		@brief ������ ��������
		*/
		virtual void trainLoop() = 0;
		virtual ~Core() {};
	};

	/**
	@brief ����� - ���� ��� ������������� "��������������������" ������
	*/
	class RandomCore : public Core {
	private:
		std::vector<std::shared_ptr<Classifyer>> _model;
	public:
		/**
		@brief ����������� ������
		@param model ������ ���������������, �������� ������������ ������
		*/
		RandomCore() {};
		RandomCore(std::vector<std::shared_ptr<Classifyer>> model) { _model = model; }
		~RandomCore() override {};

		/**
		@brief ������� ������������� "��������������������" ������
		@param t ����� ��� �������
		*/
		std::vector<double> run(textVec& t) const override;
		
		void trainLoop() override 
		{ for (auto el : _model) el->train(); };
	};
}
