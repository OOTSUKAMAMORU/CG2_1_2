#include <windows.h>
#include <cstdint>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
//�E�B���h�E�v���[�W��
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//dxCompiler��������
	IDxcUtils* dxcUils = nullptr;
	IDxcCompiller3* dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcUtils));
	assrt(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assrt(SUCCEEDED(hr));
	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr))
		//RootSignature�쐬
		D3D12_ROOT_SIGNATURE_DESC dedcriptionRootSignature{};
	descriptionRootSignature.Flags;
	D3D12_ROOT_SIIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INIPUT_LAYOUT;
	//�V���A���C�Y���ăo�C�i���ɂ���
	ID3DBlob* signatrueBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		LOG(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//�o�C�i�������ɐ���
	ID3D12RootSignature* rootSignature = nullptr;
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDesecs[0].SemanticName = "POSITION";
	inputElementDesecs[0].SemanticIndex = 0;
	inputElementDesecs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesecs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
	//BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};
	//���ׂĂ̐F�v�f����������
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//RasiterzerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//�O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	//Shader���R���p�C������
	IDxcBlob* vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);
	
	IDxcBlob* pixelShaderBlob = Compi;eShader(L"Object3D.ps.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICSPIPELINE_STaTE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	graphicsPipelineStateDesc.BlendState = blendDesc;
	//��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8GBA_UNORM_SRGB;

	//���p����g�|���W(�`��)�̃^�C�v�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOOGY_TYPE_TRIANGLE;
	//�ǂ̂悤�ɉ�ʂɐF��ł����ނ��̐ݒ�
	graphicsPipelineStateDEsc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//���ۂɐ���
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
	//���_���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC vertexResourceDesc{};

	//�o�b�t�@���\�[�X�B�e�N�X�`���̏ꍇ�͂܂��ʂ̐ݒ������
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3;
	//�o�b�t�@�̏ꍇ�͂�����1�ɂ��錈�܂�
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;

	//�o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���ۂɒ��_���\�[�X�����
	ID3D12ResourceDesc* vertexResource = nullptr;

	hr = device->CreateCommittedResource(&uploadHeapPropertoes, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOUCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	//���_�o�b�t�@�r���[���쐬����
	D3D21_VERTEX_BUFFER_VIEW vertexBufferView{};

	//���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//1���_������̃T�C�Y
	vertexBufferView.StrideInBytes = sizeof(Vector4);

	//���_���\�[�X�Ƀf�[�^����������
	Vector4* vertexData = nullptr;

	//�������ނ��߂̃A�h���X���擾
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	
	//����
	vertexResource[0] = { -0.5f,-0.5f,0.0f,1.0f };

	//��
	vertexResource[1] = { 0.0f,0.5f,0.0f,1.0f };

	//�E��
	vertexResource[2] = { 0.5f,-0.5f,0.0f,1.0f };

	//�r���[�|�[�g
	D3D12_VIEWPORT viewport{};

	//�N���C�A���g�̈�̃T�C�Y�ƈꏏ�ɂ��ĉ�ʑS�̂ɕ\��
	viewport.Width = kClientWidth;
	viewport.Height = kClientHeight;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//�V�U�[�Z�`
	D3D12_RECT scissorRect{};

	//��{�I�Ƀr���[�|�[�g�Ɠ����Z�`���\�������悤�ɂ���
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.botton = kClientHeight;

	commandList->RSSeetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
	
	//RootSignature��ݒ�BPSO�ɐݒ肹�Ă��邯�Ǖʓr�ݒ肪�K�v
	commandList->SetgraphicsRooSignature(rootSignature);
	commandList->SetPipelineState(graphicsPipelineAtate);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�`��I�B3���_�łP�̃C���X�^���X�B�C���X�^���X�ɂ��Ă͍���
	commandList->DrawInstanced(3, 1, 0, 0);

	vertexResource->Release();
	graphicsPipelineState->Releaselease();
	signatureBlob->release();
	if)(errorBlob)
	{
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();

	commandList->RSS
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	LPCWSTR arguments[] =
	{
		filePath.c_str(),
		L"-E",L"main",
		L"-Zi",L"-Qwmbed_debug",
		L"-Od",
		L"-Zpr",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	assert(SUCCEEDED(hr));
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		assert(false);
	}
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assrt(SUCCEEDED(hr));
	//�����������O���o��
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}/n", filePath, profile)));
	shaderSource->Relese();
	shaderResult->Release();
	//���s�p�̃o�C�i����ԋp
	return shaderBlob;
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}
	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);

	//windows�A�v���ł̃G���g���[�|�C���g
	int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
	{
		//�N���C�A���g�̈�̃T�C�Y
		const int32_t kClientWidth = 1200;
		const int32_t kClientHeight = 720;
		//�E�B���h�E�T�C�Y��\���\���̂ɃN���C�A���g�̈������
		RECT wrc = { 0,0,kClientWidth,kClientHeight };
		//�N���C�A���g�̈�����ƂɎ��ۂ̃T�C�Y��wrc��ύX���Ă��炤
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
		WNDCLASS wc{};
		//�E�B���h�E�v���V�[�W��
		wc.lpfnWndProc = WindowProc;
		//�E�B���h�E�N���X��(�Ȃ�ł��ǂ�)
		wc.lpszClassName = L"CG2WindowClass";
		//�C���X�^���X�n���h��
		wc.hInstance = GetModuleHandle(nullptr);
		//�J�[�\��
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		//�E�B���h�E�N���X��o�^����
		RegisterClass(&wc);
		//�o�̓E�B���h�E�ւ̕����o��
		OutputDebugStringA("Hello,DirectX!\n");
		//�N���C�A���g�̈�����Ɏ��ۂ̃T�C�Y��wrc��ύX���Ă��炤
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
		//�E�B���h�E�̐���
		HWND hwnd = CreateWindow(
			wc.lpszClassName,
			L"CG2",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wrc.right - wrc.left,
			wrc.bottom - wrc.top,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr
		);
		//�E�B���h�E��\��������
		ShowWindow(hwnd, SW_SHOW);
		MSG msg{};
		//�E�B���h�E�́~�{�^�����������܂Ń��[�v
		while (msg.message != WM_QUIT)
		{
			//Window�Ƀ��b�Z�[�W�����Ă���ŗD��ŏ���������
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else
			{
				//�Q�[���̏���
			}
		}
		return 0;
	}