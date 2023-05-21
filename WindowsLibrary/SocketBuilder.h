// ----------------------------------------------------------------------------
//! @file	SocketBuilder.h
//! @brief	�\�P�b�g�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "SocketLibrary.h"
#include "TcpConnector.h"
#include "UdpConnector.h"

namespace alt
{
	//! @class	SocketBuilder
	//! @brief	�\�P�b�g�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC SocketBuilder : public SocketLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY SocketBuilder ();

		//! @brief �f�X�g���N�^
		APIENTRY ~SocketBuilder ();

		//! @brief Windows�\�P�b�g�̎g�p����
		//! @return int
		//! @retval 0(����)
		//! @retval WSASYSNOTREADY(��b�ƂȂ�l�b�g���[�N�E�T�u�V�X�e�����A�l�b�g���[�N�ʐM�̏������ł��Ă��܂���B)
		//! @retval WSAVERNOTSUPPORTED(�v�����ꂽ Windows �\�P�b�g �T�|�[�g�̃o�[�W�����́A���̓���� Windows �\�P�b�g�����ł͒񋟂���܂���B)
		//! @retval WSAEINPROGRESS(�u���b�L���O Windows �\�P�b�g 1.1 ���삪�i�s���ł��B)
		//! @retval WSAEPROCLIM(Windows �\�P�b�g�̎����ŃT�|�[�g����Ă���^�X�N�̐��̐����ɒB���܂����B)
		//! @retval WSAEFAULT(lpWSAData �p�����[�^�[���L���ȃ|�C���^�[�ł͂���܂���B)
		//! @retval ��L�ȊO�͕s���ȃG���[
		//! @note   WSAStartup�֐��͂��̊֐��̖߂�l�Ŋg���G���[�R�[�h�𒼐ڕԂ��܂��BWSAGetLastError�֐����g�p���Ȃ��ł��������B
		int APIENTRY Startup ();

		//! @brief UdpConnector�̍쐬
		//! @param lpctszIpAddr �ݒ肷�鎩�g��IP�A�h���X
		//! @param portNo �ݒ肷�鎩�g�̃|�[�g�ԍ�
		//! @return UdpConnector*
		UdpConnector* APIENTRY CreateUdpConnector (
			LPCTSTR lpctszIpAddr, u_short portNo);

		//! @brief TcpConnector�̍쐬
		//! @param lpctszIpAddr �ݒ肷�鎩�g��IP�A�h���X
		//! @param portNo �ݒ肷�鎩�g�̃|�[�g�ԍ�
		//! @param retryInterval ���g���C�Ԋu(ms)
		//! @param retryCount ���g���C��
		//! @return TcpConnector*
		TcpConnector* APIENTRY CreateTcpConnector (
			LPCTSTR lpctszIpAddr, u_short portNo, int retryInterval,
			int retryCount);

		//! @brief TCP�|�[�g��listen(),accept()�̏���
		//! @param portNo �ҋ@�|�[�g�ԍ�
		//! @param lpctszIpAddr �ҋ@IP�A�h���X
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Prepare (u_short portNo, LPCTSTR lpctszIpAddr = NULL);

		//! @brief TCP�ڑ���listen(),accept()�ҋ@�֐�
		//! @return TcpConnector*
		TcpConnector* APIENTRY Wait ();

		//! @brief Wait()�ҋ@���̃L�����Z������
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL CancelWait ();

		//! @brief �z�X�g������IP�A�h���X��DNS����
		//! @param lpctszHostName �z�X�g��
		//! @param lptszIpAddr DNS���O�������ꂽIP�A�h���X
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY GetHostByName (
			LPCTSTR lpctszHostName, LPCTSTR lptszIpAddr) const;

	protected:
		BOOL APIENTRY Socket (int af, int type, int protocol);
		BOOL APIENTRY SetSockAddr (
			ADDRESS_FAMILY family, u_short portNo, LPCTSTR lpctszAddress);
		BOOL APIENTRY SetSockOpt (int level, int optname, int value) const;
		BOOL APIENTRY Connect () const;
		BOOL APIENTRY Bind () const;
		BOOL APIENTRY Listen () const;
		BOOL APIENTRY Ioctl (long cmd, u_long arg) const;

		SOCKADDR_IN	_SockAddrIn;
		WSADATA		_wsaData;
		WSAEVENT _wsaEvent;
	};
}